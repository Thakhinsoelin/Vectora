#include "Application.h"
#include "vpch.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "GLFW/glfw3.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Vectora {
	Application::Application()
	{
		window = std::unique_ptr<Window>(Window::Create());
		window.get()->SetEventCallback(
			[&](Event& e) {this->OnEvent(e); }
		);
		window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		VE_CORE_INFO("{0}", e);
	}

	
}