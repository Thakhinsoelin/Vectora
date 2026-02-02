#include "Vectora.h"
#include "imgui.h"
#include "imgui/ImGuiLayer.h"

class TestLayer : public Vectora::Layer {
public:
    TestLayer()
    : Layer("Example"){

    }
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
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

        // SYNC CONTEXT: This prevents the Segfault
        auto* imguiLayer = (Vectora::ImGuiLayer*)Vectora::ImGuiLayer::GetImguiLayerInstance();
        ImGui::SetCurrentContext(imguiLayer->GetContext());
    }
    ~SandBox() {

    }
};

Vectora::Application* Vectora::CreateApplication() {
    return new SandBox();
}
