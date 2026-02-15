#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "imgui/ImGuiLayer.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "OrthographiCamera.h"
#include "Core/Timestep.h"
#include <math.h>

namespace Vectora {
	class VECTORA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		inline Window& GetWindow() { return *window; }

		inline static Application& Get() { return *s_Instance; }

		void SetRunning(bool run) {
			this->m_Running = run;
		}
	private:
		Scope<Window> window;
		ImGuiLayer* m_ImguiLayer;
		LayerStack layerstack;
		bool m_Running = true;
		bool m_Minimized = false;
		static Application* s_Instance;
		
		float m_LastFrameTime = 0.f;
	};
	Application* CreateApplication();
}

