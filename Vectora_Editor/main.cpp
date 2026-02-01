#include "Vectora.h"

class TestLayer : public Vectora::Layer {
public:
    TestLayer()
    : Layer("Example"){

    }

    void OnUpdate() override {
		/*if (Vectora::Input::IsKeyPressed(VE_KEYCODE::VE_KEY_SPACE)) {
			VE_CORE_INFO("Space Key Pressed!");
		}*/
		VE_CORE_INFO("TestLayer::Update");
    }

    void OnEvent(Vectora::Event& event) override {
        VE_TRACE("{0}", event);
    }
};

class SandBox : public Vectora::Application {
public:
    SandBox() {
		PushLayer(new TestLayer() );
        PushOverlay(new Vectora::ImGuiLayer());
    }
    ~SandBox() {

    }
};

Vectora::Application* Vectora::CreateApplication() {
    return new SandBox();
}
