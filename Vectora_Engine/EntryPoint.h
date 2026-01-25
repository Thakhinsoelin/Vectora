#pragma once
#ifdef V_PLATFORM_WINDOWS

extern Vectora::Application* Vectora::CreateApplication();

int main(int argc, char** argv) {
    Vectora::Log::Init();
    V_CORE_WARN("initialied!");
    int a = 3;
    V_INFO("Hello var={0}", a);
    auto app = Vectora::CreateApplication();
    app->Run();

    delete app;
    return 0;
}
#endif