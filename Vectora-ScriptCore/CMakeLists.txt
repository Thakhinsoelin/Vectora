cmake_minimum_required(VERSION 3.28)
project(Vectora-Scripting LANGUAGES CSharp)

# Set the .NET SDK style
set(CMAKE_DOTNET_SDK "Microsoft.NET.Sdk")

# Fallback defaults if not supplied by the main C++ CMake call
if(NOT ENGINE_BINARY_DIR)
    set(ENGINE_BINARY_DIR "${CMAKE_SOURCE_DIR}/../build")
endif()
if(NOT CONFIG)
    set(CONFIG "Debug")
endif()
if(NOT PLATFORM)
    set(PLATFORM "Win64")
endif()

# The unified output directory where the C++ engine runs
set(FINAL_OUT_DIR "${ENGINE_BINARY_DIR}/bin/${CONFIG}-${PLATFORM}/")

# =====================================================================
# 1. Vectora-ScriptCore (C# API Library)
# =====================================================================
# Sources are located in: root/Vectora-ScriptCore/Source
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cs")

add_library(Vectora-ScriptCore SHARED ${CORE_SOURCES})

set_property(TARGET Vectora-ScriptCore PROPERTY DOTNET_TARGET_FRAMEWORK "net472")
set_target_properties(Vectora-ScriptCore PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"

    VS_GLOBAL_OutputPath "${FINAL_OUT_DIR}"
    VS_GLOBAL_OutDir "${FINAL_OUT_DIR}"
)

# =====================================================================
# 2. Sandbox-Scripts (C# Gameplay/Application Scripts)
# =====================================================================
# Target folder: root/Vectora_Editor/SandboxProj/Assets/Scripts/Sources
set(SANDBOX_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Vectora_Editor/SandboxProj/Assets/Scripts/Source")

file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS "${SANDBOX_SCRIPTS_DIR}/*.cs")

add_library(Sandbox-Scripts SHARED ${SANDBOX_SOURCES})

set_property(TARGET Sandbox-Scripts PROPERTY DOTNET_TARGET_FRAMEWORK "net472")
set_target_properties(Sandbox-Scripts PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"

    VS_GLOBAL_OutputPath "${FINAL_OUT_DIR}"
    VS_GLOBAL_OutDir "${FINAL_OUT_DIR}"
)

# --- Link the Sandbox Scripts to the Core C# API ---
target_link_libraries(Sandbox-Scripts PUBLIC Vectora-ScriptCore)