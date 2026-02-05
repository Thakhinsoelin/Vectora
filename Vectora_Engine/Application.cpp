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
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Vectora::ShaderDataType::Float:    return GL_FLOAT;
		case Vectora::ShaderDataType::Float2:   return GL_FLOAT;
		case Vectora::ShaderDataType::Float3:   return GL_FLOAT;
		case Vectora::ShaderDataType::Float4:   return GL_FLOAT;
		case Vectora::ShaderDataType::Mat3:     return GL_FLOAT;
		case Vectora::ShaderDataType::Mat4:     return GL_FLOAT;
		case Vectora::ShaderDataType::Int:      return GL_INT;
		case Vectora::ShaderDataType::Int2:     return GL_INT;
		case Vectora::ShaderDataType::Int3:     return GL_INT;
		case Vectora::ShaderDataType::Int4:     return GL_INT;
		case Vectora::ShaderDataType::Bool:     return GL_BOOL;
		}

		VE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
	Application::Application()
	{
		VE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_ImguiLayer = new ImGuiLayer();
		PushOverlay(m_ImguiLayer);

		m_Shader = std::make_unique<Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
		m_Shader->createShaders(BOTH_FROM_FILE);
		m_Shader->Bind();
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		
		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			/*glDrawArrays(GL_TRIANGLES, 0, 3);*/
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			m_Shader->Bind();
			for (auto& layer : layerstack)
				layer->OnUpdate();
			
			m_ImguiLayer->Begin();
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