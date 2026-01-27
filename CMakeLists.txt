cmake_minimum_required(VERSION 3.28)
project(Vectora LANGUAGES CXX)
set(V_ENGINE_TARGET Vectora_Engine)
set(V_EDITOR_TARGET Vectora_Editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    GIT_TAG 1.0.3  # or a newer stable tag if you want
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0 # Use a stable version tag
    GIT_SHALLOW TRUE
)

# Disable extras we don't need
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw glm spdlog)

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

add_library(${V_ENGINE_TARGET} SHARED ${ENGINE})

# Include your own headers
target_include_directories(${V_ENGINE_TARGET} PUBLIC
    ${CMAKE_SOURCE_DIR}/Vectora_Engine
    ${CMAKE_SOURCE_DIR}/includes
)

target_compile_definitions(${V_ENGINE_TARGET} PRIVATE V_BUILD_DLL V_PLATFORM_WINDOWS)

# Link libraries
target_link_libraries(${V_ENGINE_TARGET} PUBLIC
    glfw
    glm::glm
    spdlog::spdlog
    OpenGL::GL
)

add_executable(${V_EDITOR_TARGET} ${EDITOR})
target_include_directories(${V_EDITOR_TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/Vectora_Editor
    ${CMAKE_SOURCE_DIR}/includes
)
target_compile_definitions(${V_EDITOR_TARGET} PRIVATE V_PLATFORM_WINDOWS)
target_link_libraries(${V_EDITOR_TARGET} PRIVATE ${V_ENGINE_TARGET})



# add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
#     )