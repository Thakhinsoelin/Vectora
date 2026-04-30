# Enable C# support
project(Sandbox-Scripts LANGUAGES CSharp)

# Set the .NET SDK style
set(CMAKE_DOTNET_SDK "Microsoft.NET.Sdk")

# Sources - Target your Sandbox folder where player scripts live
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cs")

# Create the assembly (Library)
add_library(Sandbox-Scripts SHARED ${SOURCES})

# Specify target framework (Must match Vectora-ScriptCore for compatibility)
set_property(TARGET Sandbox-Scripts PROPERTY DOTNET_TARGET_FRAMEWORK "net472")

# --- REFERENCE THE CORE API ---
# This is the "Separation" part: Sandbox needs to know about ScriptCore
target_link_libraries(Sandbox-Scripts PUBLIC Vectora-ScriptCore)

# Ensure the output goes to the same unified folder as the engine and Core API
set_target_properties(Sandbox-Scripts PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}"
)