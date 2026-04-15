cmake_minimum_required(VERSION 3.28)
project(Vectora LANGUAGES C CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(Vectora-ScriptCore)


set(VE_ENGINE_TARGET Vectora_Engine)
set(VE_SANDBOX_TARGET Sandbox)
set(VE_EDITOR_TARGET Vectora_Editor)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DISCORD_SDK_ROOT "${CMAKE_SOURCE_DIR}/Dependencies/Discord")
set(DISCORD_SDK_LIB_DIR "${DISCORD_SDK_ROOT}/lib/debug")
set(DISCORD_SDK_BIN_DIR "${DISCORD_SDK_ROOT}/bin/debug")
set(DISCORD_SDK_INCLUDE_DIR "${DISCORD_SDK_ROOT}/include")

# --- Mono SDK (Static Setup) ---
set(MONO_ROOT "${CMAKE_SOURCE_DIR}/Dependencies/mono")
set(MONO_INCLUDE_DIR "${MONO_ROOT}/include")
set(MONO_LIB_DIR "${MONO_ROOT}/lib")
set(MONO_STATIC_LIB "${MONO_LIB_DIR}/libmono-static-sgen.lib")

# Platform-specific Social SDK library paths tho currently windows is only supported for building out of the box
if(WIN32)
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/discord_partner_sdk.lib")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_BIN_DIR}/discord_partner_sdk.dll")
elseif(APPLE)
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
else() # Linux
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
endif()


# This ensures /MD for Release and /MDd for Debug
# I don't know how to make cmake link statically for other platforms, but for windows visual studio msvc, this is how you do it.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") 

if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ENGINE_PLATFORM "Win64")
    else()
        set(ENGINE_PLATFORM "Win32")
    endif()
elseif (APPLE)
    set(ENGINE_PLATFORM "Mac")
elseif (UNIX)
    set(ENGINE_PLATFORM "Linux")
endif()

set(BASE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/$<CONFIG>-${ENGINE_PLATFORM}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BASE_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BASE_OUTPUT_DIR})

# Note that, dynamic fetching required libraries like this is not a good idea for production code,
# and you should always consider vendoring the dependencies in your repository. This is just for learning purposes so it is fine.
# having network issues can make this fail and vomit weird errors you cannot debug.
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking # Note that this is a bad idea to target an always changing branch, and you always only should use a specific tag.
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.9.0
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    ImGuizmo
    GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo.git
    GIT_TAG master # this is a bad idea
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG        v3.1.1 # Latest one as i am writing
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw glm spdlog imgui yaml-cpp ImGuizmo box2d)
get_property(glfwTargets
    DIRECTORY ${glfw_BINARY_DIR}
    PROPERTY BUILDSYSTEM_TARGETS
)
foreach(target
    uninstall
    update_mappings
)
    if(TARGET ${target})
        set_target_properties(${target}
            PROPERTIES FOLDER "Dependencies/"
        )
    endif()
endforeach()

add_library(imgui_static STATIC 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_static PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_static PRIVATE
    glfw
    OpenGL::GL
)
target_compile_definitions(imgui_static PRIVATE GLFW_INCLUDE_NONE)

# --- OpenGL ---
find_package(OpenGL REQUIRED)

set_target_properties(glm PROPERTIES FOLDER "Dependencies")
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
set_target_properties(spdlog PROPERTIES FOLDER "Dependencies")
set_target_properties(imgui_static PROPERTIES FOLDER "Dependencies")
set_target_properties(yaml-cpp PROPERTIES FOLDER "Dependencies")
set_target_properties(yaml-cpp-parse PROPERTIES FOLDER "Dependencies")
set_target_properties(yaml-cpp-read PROPERTIES FOLDER "Dependencies")
set_target_properties(yaml-cpp-sandbox PROPERTIES FOLDER "Dependencies")
set_target_properties(box2d PROPERTIES FOLDER "Dependencies")


file(GLOB_RECURSE ENGINE CONFIGURE_DEPENDS
    ${imguizmo_SOURCE_DIR}/ImGuizmo.h
    ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
    Vectora_Engine/*.cpp
    Vectora_Engine/*.c
    Vectora_Engine/*.h
    Vectora_Engine/*.hpp
    Vectora_Engine/*.glsl # adding the glsl files to your target makes cmake also track that files so after the shader gets modified, cmake generation gets triggered again
)

file(GLOB_RECURSE EDITOR CONFIGURE_DEPENDS
    ${imguizmo_SOURCE_DIR}/ImGuizmo.h
    ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
    Vectora_Editor/*.cpp
    Vectora_Editor/*.c
    Vectora_Editor/*.h
    Vectora_Editor/*.hpp
)

file(GLOB_RECURSE SANDBOX CONFIGURE_DEPENDS
    Sandbox/*.cpp
    Sandbox/*.c
    Sandbox/*.h
    Sandbox/*.hpp
)

add_library(${VE_ENGINE_TARGET} STATIC ${ENGINE})
add_dependencies(${VE_ENGINE_TARGET} imgui_static yaml-cpp box2d Vectora-ScriptCore)
# Include your own headers
target_include_directories(${VE_ENGINE_TARGET} PUBLIC
    ${imguizmo_SOURCE_DIR}
    ${DISCORD_SDK_INCLUDE_DIR}
    ${MONO_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Vectora_Engine
    ${CMAKE_SOURCE_DIR}/includes/entt
    ${CMAKE_SOURCE_DIR}/includes
    ${imgui_SOURCE_DIR}
)

target_compile_definitions(${VE_ENGINE_TARGET} PRIVATE 
    IMGUI_DEFINE_MATH_OPERATORS 
    VE_BUILD_STATIC 
    VE_PLATFORM_WINDOWS 
    GLFW_INCLUDE_NONE
    NOMINMAX # fuck you microsoft
    MONO_INSTALLED
    $<$<CONFIG:Debug>:VE_ENABLE_ASSERTS>
)

set_target_properties(${VE_ENGINE_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}"
)

# Link libraries
target_link_libraries(${VE_ENGINE_TARGET} PUBLIC
    ${DISCORD_LIB_PATH}
    ${MONO_STATIC_LIB}           # <--- Mono Static Library
    # --- Windows System Dependencies for Mono ---
    Bcrypt.lib                   # Cryptography/Hashing
    version.lib                  # File versioning
    Winmm.lib                    # High-precision timers
    Ws2_32.lib                   # Networking/Sockets
    Rpcrt4.lib                   # RPC (Required for Mono threading/UUIDs)
    Shlwapi.lib                  # Shell API (Path handling)
    
    yaml-cpp
    imgui_static
    glfw
    glm::glm
    spdlog::spdlog
    OpenGL::GL
    box2d
) # // BREAKPOINT

target_include_directories(${VE_ENGINE_TARGET} SYSTEM PRIVATE
    $<TARGET_PROPERTY:yaml-cpp,INTERFACE_INCLUDE_DIRECTORIES>
)

target_precompile_headers(${VE_ENGINE_TARGET} PRIVATE 
    "$<$<COMPILE_LANGUAGE:CXX>:vpch.h>"
)

add_executable(${VE_EDITOR_TARGET} ${EDITOR} ${CMAKE_SOURCE_DIR}/Vectora_Editor/resource.rc)

target_include_directories(${VE_EDITOR_TARGET} PRIVATE
    ${imguizmo_SOURCE_DIR}
    ${DISCORD_SDK_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Vectora_Editor
    ${CMAKE_SOURCE_DIR}/includes/entt
    ${CMAKE_SOURCE_DIR}/includes
)

set_target_properties(${VE_EDITOR_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}"
)

target_compile_definitions(${VE_EDITOR_TARGET} PRIVATE VE_BUILD_STATIC VE_PLATFORM_WINDOWS)
target_link_libraries(${VE_EDITOR_TARGET} PUBLIC ${VE_ENGINE_TARGET} ${DISCORD_LIB_PATH})

add_executable(${VE_SANDBOX_TARGET} ${SANDBOX})
target_include_directories(${VE_SANDBOX_TARGET} PRIVATE
    ${imguizmo_SOURCE_DIR}
    ${DISCORD_SDK_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Sandbox
    ${CMAKE_SOURCE_DIR}/Vectora_Engine
    ${CMAKE_SOURCE_DIR}/includes/entt
    ${CMAKE_SOURCE_DIR}/includes
)

set_target_properties(${VE_SANDBOX_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}"
)

target_compile_definitions(${VE_SANDBOX_TARGET} PRIVATE VE_BUILD_STATIC VE_PLATFORM_WINDOWS)
target_link_libraries(${VE_SANDBOX_TARGET} PUBLIC ${VE_ENGINE_TARGET} ${DISCORD_LIB_PATH})



# Forcing the cmake build output
#${CMAKE_BINARY_DIR}/bin/$<CONFIG>-${ENGINE_PLATFORM}
set_target_properties(${VE_EDITOR_TARGET} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>-${ENGINE_PLATFORM}"
)

set_target_properties(${VE_SANDBOX_TARGET} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>-${ENGINE_PLATFORM}"
)

# to copy the entire shaders to the output path, i am not sure it is a good idea or not
add_custom_target(copy_shaders ALL
    # Ensure the destination directories exist first
    #COMMAND ${CMAKE_COMMAND} -E make_directory "${BASE_OUTPUT_DIR}/shaders"
    #COMMAND ${CMAKE_COMMAND} -E make_directory "${BASE_OUTPUT_DIR}/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DISCORD_SHARED_LIB}"
            "${BASE_OUTPUT_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_directory "${MONO_LIB_DIR}/mono" "${BASE_OUTPUT_DIR}/mono"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Vectora_Engine/shaders"
            "${BASE_OUTPUT_DIR}/shaders"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Vectora_Editor/Resources"
            "${BASE_OUTPUT_DIR}/Resources"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Sandbox/assets"
            "${BASE_OUTPUT_DIR}/assets"
)

set_target_properties(copy_shaders PROPERTIES FOLDER "Dependencies")

add_dependencies(${VE_EDITOR_TARGET} copy_shaders) 