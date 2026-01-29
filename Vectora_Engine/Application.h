#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"


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
	private:
		std::unique_ptr<Window> window;
		LayerStack layerstack;
		bool m_Running = true;
	};
	Application* CreateApplication();
}

