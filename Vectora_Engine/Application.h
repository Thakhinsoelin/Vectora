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

		inline Window& GetWindow() { return *window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Window> window;
		bool onmove(KeyPressedEvent& e);
		ImGuiLayer* m_ImguiLayer;
		glm::vec3 tempPos = { 0.5, 0.5, 0.0 };
		LayerStack layerstack;
		bool m_Running = true;
		static Application* s_Instance;
		OrthoGraphicCamera m_Camera;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> m_Shader;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_BlueShader;

		
		// std::vector<Ray> rays;
		// BlacHole SagA;
	};
	Application* CreateApplication();
}

