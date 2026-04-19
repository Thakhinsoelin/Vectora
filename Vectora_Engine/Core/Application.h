#pragma once

#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/LayerStack.h"
#include "Core/Timestep.h"
#include "imgui/ImGuiLayer.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/OrthographiCamera.h"

#ifdef SUPPORT_DISCORD
#include "Core/DiscordManager.h"
#endif

#include <math.h>

int main(int argc, char** argv);

namespace Vectora {
	struct ApplicationSpecification
	{
		std::string Name = "Vectora Application";
		std::string WorkingDirectory;
	};

	class VECTORA_API Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		inline Window& GetWindow() { return *window; }

		inline static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() { return m_Specification; }
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
#ifdef SUPPORT_DISCORD
		Scope<DiscordManager> m_DiscordManager = nullptr;
#endif		

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
		ApplicationSpecification m_Specification;
	};
	Application* CreateApplication();
}

