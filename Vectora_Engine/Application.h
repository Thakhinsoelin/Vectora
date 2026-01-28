#pragma once
#include "Core.h"
#include "Window.h"


namespace Vectora {
	class VECTORA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
	private:
		std::unique_ptr<Window> window;
		bool m_Running = true;
	};
	Application* CreateApplication();
}

