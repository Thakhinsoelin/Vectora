cmake_minimum_required(VERSION 3.28)
project(Vectora LANGUAGES CXX)
set(VE_ENGINE_TARGET Vectora_Engine)
set(VE_EDITOR_TARGET Vectora_Editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#I might need this later
# This ensures /MD for Release and /MDd for Debug
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL") 

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

foreach(OUTPUTCONFIG Debug Release RelWithDebInfo MinSizeRel)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}-${ENGINE_PLATFORM})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}-${ENGINE_PLATFORM})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
        ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}-${ENGINE_PLATFORM})
endforeach()

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
    GIT_TAG docking
    GIT_SHALLOW TRUE
)


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw glm spdlog imgui)
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

file(GLOB_RECURSE ENGINE CONFIGURE_DEPENDS
    Vectora_Engine/*.cpp
    Vectora_Engine/*.c
    Vectora_Engine/*.h
)

file(GLOB_RECURSE EDITOR CONFIGURE_DEPENDS
    Vectora_Editor/*.cpp
    Vectora_Editor/*.c
    Vectora_Editor/*.h
)



add_library(${VE_ENGINE_TARGET} STATIC ${ENGINE})
add_dependencies(${VE_ENGINE_TARGET} imgui_static)
# Include your own headers
target_include_directories(${VE_ENGINE_TARGET} PUBLIC
    ${CMAKE_SOURCE_DIR}/Vectora_Engine
    ${CMAKE_SOURCE_DIR}/includes
    ${imgui_SOURCE_DIR}
)

target_compile_definitions(${VE_ENGINE_TARGET} PRIVATE VE_BUILD_STATIC VE_PLATFORM_WINDOWS GLFW_INCLUDE_NONE)
target_compile_definitions(${VE_ENGINE_TARGET}
    PRIVATE
        $<$<CONFIG:Debug>:VE_ENABLE_ASSERTS>
)



# Link libraries
target_link_libraries(${VE_ENGINE_TARGET} PUBLIC
    imgui_static
    glfw
    glm::glm
    spdlog::spdlog
    OpenGL::GL
)

target_precompile_headers(${VE_ENGINE_TARGET} PRIVATE 
    "$<$<COMPILE_LANGUAGE:CXX>:vpch.h>"
)

add_executable(${VE_EDITOR_TARGET} ${EDITOR})
target_include_directories(${VE_EDITOR_TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/Vectora_Editor
    ${CMAKE_SOURCE_DIR}/includes
)
target_compile_definitions(${VE_EDITOR_TARGET} PRIVATE VE_BUILD_STATIC VE_PLATFORM_WINDOWS)
target_link_libraries(${VE_EDITOR_TARGET} PUBLIC ${VE_ENGINE_TARGET})

foreach(OUTPUTCONFIG Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${OUTPUTCONFIG} CONFIG_UPPER)
    set_target_properties(${VE_EDITOR_TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}-${ENGINE_PLATFORM}"
    )
    set_target_properties(${VE_ENGINE_TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}-${ENGINE_PLATFORM}"
        LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}-${ENGINE_PLATFORM}"
        ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG}-${ENGINE_PLATFORM}"
    )
endforeach()

#${CMAKE_BINARY_DIR}/bin/$<CONFIG>-${ENGINE_PLATFORM}
set_target_properties(${VE_EDITOR_TARGET} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>-${ENGINE_PLATFORM}"
)

add_custom_command(
    TARGET ${VE_EDITOR_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${CMAKE_SOURCE_DIR}/Vectora_Engine/shaders"
            "$<TARGET_FILE_DIR:${VE_EDITOR_TARGET}>/shaders"
)