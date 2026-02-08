#include "Application.h"
#include "vpch.h"
#include "Events/Event.h"
#include "Input.h"
#include "Events/ApplicationEvent.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include <imgui.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

// This is stupid. C++ 20 magic
auto do_something() -> auto {
	return Vectora::Application();
}

namespace Vectora {
	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		VE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		window = std::unique_ptr<Window>(Window::Create(/*WindowProps("test",800, 800 ))*/));
		window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_ImguiLayer = new ImGuiLayer();
		PushOverlay(m_ImguiLayer);

		m_Shader = std::make_shared<Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
		m_Shader->createShaders(BOTH_FROM_FILE);
		m_Shader->Bind();
		
		m_BlueShader = std::make_shared<Shader>("shaders/blueRectVt.glsl", "shaders/blueRectFg.glsl");
		m_BlueShader->createShaders(BOTH_FROM_FILE);
		

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexArray.reset(VertexArray::Create());

		
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		

		/*SagA = BlacHole(glm::vec3(0.f, 0.f, 0.f), 8.54e36f);

		for (float i = -1.f; i <= 1.f; i += 0.05f)
		{
			rays.push_back(Ray(glm::vec2(-0.5f, i), glm::vec2(1.0f, 0.f)));

		}*/

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		/*glm::vec2 offset(0.5f, 0.f);*/
		glm::vec2 offset(0.0f, 0.f);
		while (m_Running) {
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_BlueShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			/*glDrawArrays(GL_TRIANGLES, 0, 3);*/
			
			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			
			
			//SagA.draw();

			//for (auto& e : rays) {
			//	//glLineWidth(3.0f);
			//	e.draw();
			//	//e.step(SagA.radius);
			//	e.gpt_step(SagA.radius, glm::vec2(SagA.position));
			//}

			for (auto& layer : layerstack)
				layer->OnUpdate();
			
			
			m_ImguiLayer->Begin();
			//ImGui::Begin("bla");
			//ImGui::Text("Hello World");
			//if (ImGui::Button("Restart")) {
			//	int counter = 0;
			//	for (float i = -1.f; i <= 1.f; i += 0.05f)
			//	{
			//		//rays.push_back(Ray(glm::vec2(-0.5f, i), glm::vec2(1.0f, 0.f)));
			//		rays[counter] = Ray(glm::vec2(-0.5f, i), glm::vec2(1.0f, 0.f));
			//		counter++;
			//	}
			//};
			//ImGui::End();
			for (Layer* layer: layerstack)
			{
				layer->OnImGuiRender();
			}
			m_ImguiLayer->End();

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		//if(e.GetEventType() == EventType::MouseButtonPressed || e.GetEventType() == EventType::MouseButtonReleased)
			//VE_CORE_TRACE("{0}", e);
		
		for (auto it = layerstack.end(); it != layerstack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		layerstack.PushLayer(layer);
		
	}

	void Application::PushOverlay(Layer* overlay)
	{
		layerstack.PushOverlay(overlay);
		
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	
}