#pragma once
#ifdef VE_PLATFORM_WINDOWS

extern Vectora::Application* Vectora::CreateApplication();

int main(int argc, char** argv) {
    Vectora::Log::Init();
    VE_CORE_WARN("initialied!");
    int a = 3;
    VE_INFO("Hello var={0}", a);
    auto app = Vectora::CreateApplication();
    app->Run();

    delete app;
    return 0;
}
#endif