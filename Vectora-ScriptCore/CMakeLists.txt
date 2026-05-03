cmake_minimum_required(VERSION 3.28)
# Enable C# support
project(Vectora-ScriptCore LANGUAGES CSharp)

# Set the .NET SDK style (Modern way)
set(CMAKE_DOTNET_SDK "Microsoft.NET.Sdk")

# Use the variables passed from the main engine call
# We default them in case you open this project standalone
if(NOT ENGINE_BINARY_DIR)
    set(ENGINE_BINARY_DIR "${CMAKE_SOURCE_DIR}/../build")
endif()

set(FINAL_OUT_DIR "${ENGINE_BINARY_DIR}/bin/${CONFIG}-${PLATFORM}")

# Sources
file(GLOB_RECURSE  SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cs")

# Create the assembly (Library)
add_library(Vectora-ScriptCore SHARED ${SOURCES})

# Specify target framework (Mono works best with net472 or netstandard2.0)
set_property(TARGET Vectora-ScriptCore PROPERTY DOTNET_TARGET_FRAMEWORK "net472")
set_target_properties(Vectora-ScriptCore PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${FINAL_OUT_DIR}"
)