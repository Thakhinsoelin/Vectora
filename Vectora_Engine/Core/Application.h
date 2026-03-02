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
#include "Renderer/OrthographiCamera.h"
#include "Core/Timestep.h"
#include <math.h>

int main(int argc, char** argv);

namespace Vectora {
	class VECTORA_API Application
	{
	public:
		Application(const std::string& name = "Vectora Main App");
		virtual ~Application();

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

		ImGuiLayer* GetImGuiLayer() { return m_ImguiLayer; }
	private:
		void Run();
		Scope<Window> window;
		ImGuiLayer* m_ImguiLayer;
		LayerStack layerstack;
		bool m_Running = true;
		bool m_Minimized = false;
		
		float m_LastFrameTime = 0.f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};
	Application* CreateApplication();
}

