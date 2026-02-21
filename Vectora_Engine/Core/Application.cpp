#include "Core/Application.h"
#include "vpch.h"
#include "Events/Event.h"
#include "Core/Input.h"
#include "Events/ApplicationEvent.h"
#include "Debug/Instrumentor.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"
#include <imgui.h>
//#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

// This is stupid. C++ 20 magic
auto do_something() -> auto {
	return 3;
	//return Vectora::Application();
}

namespace Vectora {
	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		VE_PROFILE_FUNCTION();

		VE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		window = Window::Create({ "Test", 600,600 });
		window->SetVSync(false);
		window->SetEventCallback(VE_BIND_EVENT_FN(Application::OnEvent));
		//keep track this
		//layerstack = LayerStack();
		Renderer::Init();

		m_ImguiLayer = new ImGuiLayer();
		PushOverlay(m_ImguiLayer);
	}

	Application::~Application()
	{
		VE_PROFILE_FUNCTION();
		Renderer::Shutdown();
	}

	void Application::Run()
	{
		VE_PROFILE_FUNCTION();
		while (m_Running) {
			VE_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			//if(!m_Minimized)
			{
				VE_PROFILE_SCOPE("layerstack onupdate");
				for (auto& layer : layerstack)
					layer->OnUpdate(timestep);
			}
			
			m_ImguiLayer->Begin();
			{
				VE_PROFILE_SCOPE("layerstack on imgui render");
				for (Layer* layer : layerstack)
				{
					layer->OnImGuiRender();
				}
			}
			m_ImguiLayer->End();

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		VE_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VE_BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(VE_BIND_EVENT_FN(Application::OnWindowResized));
		for (auto it = layerstack.rbegin(); it != layerstack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		VE_PROFILE_FUNCTION();
		layerstack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		VE_PROFILE_FUNCTION();
		layerstack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}