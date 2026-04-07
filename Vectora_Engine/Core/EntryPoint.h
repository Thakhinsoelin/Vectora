#pragma once
#ifdef VE_PLATFORM_WINDOWS
#include "Core/CmdArgumentHandler.h"
#include "Utils/PlatformUtils.h"
#include <filesystem>

extern Vectora::Application* Vectora::CreateApplication();

int main(int argc, char** argv) {
    
    Vectora::CmdArgumentHandler::Init(argc, argv);

    /*std::string path = argv[0];
    std::string directory;
    size_t last_slash_idx = path.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
        directory = path.substr(0, last_slash_idx);
    }
    std::filesystem::current_path(directory);*/
    std::filesystem::current_path(Vectora::FileDialogs::GetExecutablePath());
    Vectora::Log::Init();
    //VE_WARN("Current Working directoy of the editor is {0}", std::filesystem::current_path().string());
    
    //fuck you windows, why tf you think force changing the working directory is a good idea.
//    VE_WARN("Current Working directoy of the editor after fix is {0}", std::filesystem::current_path().string());
    VE_PROFILE_BEGIN_SESSION("Startup", "VectoraProfile-Startup.json");
    auto app = Vectora::CreateApplication();
    VE_PROFILE_END_SESSION();

    VE_PROFILE_BEGIN_SESSION("Runtime", "VectoraProfile-Runtime.json");
    app->Run();
    VE_PROFILE_END_SESSION();

    VE_PROFILE_BEGIN_SESSION("Shutdown", "VectoraProfile-Shutdown.json");
    delete app;
    VE_PROFILE_END_SESSION();
    return 0;
}
#endif