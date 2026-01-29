#include "Vectora.h"

class TestLayer : public Vectora::Layer {
public:
    TestLayer()
    : Layer("Example"){

    }

    void OnUpdate() override {
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
    }
    ~SandBox() {

    }
};

Vectora::Application* Vectora::CreateApplication() {
    return new SandBox();
}
