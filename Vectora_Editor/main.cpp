#include "Vectora.h"
#include "imgui.h"
#include "imgui/ImGuiLayer.h"

class TestLayer : public Vectora::Layer {
public:
    TestLayer()
    : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		
		m_Shader = std::make_shared<Vectora::Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
		m_Shader->createShaders(Vectora::BOTH_FROM_FILE);
		m_Shader->Bind();

		m_BlueShader = std::make_shared<Vectora::Shader>("shaders/blueRectVt.glsl", "shaders/blueRectFg.glsl");
		m_BlueShader->createShaders(Vectora::BOTH_FROM_FILE);


		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexArray.reset(Vectora::VertexArray::Create());


		Vectora::BufferLayout layout = {
			{ Vectora::ShaderDataType::Float3, "a_Position" },
			{ Vectora::ShaderDataType::Float4, "a_Color" }
		};

		std::shared_ptr<Vectora::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Vectora::VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Vectora::IndexBuffer> indexBuffer;
		indexBuffer.reset(Vectora::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Vectora::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Vectora::VertexBuffer> squareVB;
		squareVB.reset(Vectora::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Vectora::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Vectora::IndexBuffer> squareIB;
		squareIB.reset(Vectora::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
    }

    void OnImGuiRender() override
    {
		ImGui::Begin("Test");
		ImGui::Text("Hello\n");
		ImGui::End();
    }

    void OnUpdate(Vectora::Timestep ts) override {
		VE_TRACE("time: {0}", ts.GetMilliseconds());
		if (Vectora::Input::IsKeyPressed(VE_KEY_LEFT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		else if (Vectora::Input::IsKeyPressed(VE_KEY_RIGHT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		if (Vectora::Input::IsKeyPressed(VE_KEY_DOWN))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Vectora::Input::IsKeyPressed(VE_KEY_UP))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Vectora::Input::IsKeyPressed(VE_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Vectora::Input::IsKeyPressed(VE_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Vectora::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Vectora::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Vectora::Renderer::BeginScence(m_Camera);
		m_BlueShader->Bind();
		Vectora::Renderer::Submit(m_BlueShader, m_SquareVA);

		m_SquareVA->Bind();
		Vectora::Renderer::Submit(m_Shader, m_VertexArray);

		Vectora::Renderer::EndScene();
    }

    void OnEvent(Vectora::Event& event) override {
    
    }
private:
	Vectora::OrthoGraphicCamera m_Camera;

	std::shared_ptr<Vectora::VertexArray> m_VertexArray;
	std::shared_ptr<Vectora::Shader> m_Shader;
					
	std::shared_ptr<Vectora::VertexArray> m_SquareVA;
	std::shared_ptr<Vectora::Shader> m_BlueShader;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 1.f;
	float m_CameraRotation = 0.f;
	float m_CameraRotationSpeed = 180.f;
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
