#include "WindowsWindow.h"
#include "vpch.h"
#include "Core/Log.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "platforms/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace Vectora {

	static VE_UINT8 s_GLFWWindowCount = 0;
	static void GLFWErrorCallback(int error, const char* desc) {
		VE_CORE_ERROR("GLFW Error ({0}): ({1})", error, desc);
	}
	Scope<Window> Window::Create(const WindowProps& prop) {
		return CreateScope<WindowsWindow>(prop);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}
	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
		
	}
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
	void WindowsWindow::Init(const WindowProps& prop)
	{
		this->m_Data.Title = prop.title;
		this->m_Data.Width = prop.width;
		this->m_Data.Height = prop.height;

		VE_CORE_INFO("Creating window {0} ({1}, {2})", prop.title, prop.width, prop.height);
		if (s_GLFWWindowCount == 0)
		{
			// TODO: glfwTerminate on system shutdown
			VE_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			VE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			

		}
		m_Window = glfwCreateWindow(prop.width, prop.height, prop.title, nullptr, nullptr);
		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(static_cast<VE_KEYCODE>(key), 0);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(static_cast<VE_KEYCODE>(key));
					data.EventCallback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(static_cast<VE_KEYCODE>(key), 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(static_cast<VE_KEYCODE>(key));
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(static_cast<VE_KEYCODE>(button));
				data.EventCallback(event);
				break;
			}

			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(static_cast<VE_KEYCODE>(button));
				data.EventCallback(event);
				break;
			}
		}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}
	void WindowsWindow::Shutdown()
	{
		if (m_Window != nullptr) {
			glfwDestroyWindow(m_Window);
		}
		if (--s_GLFWWindowCount == 0)
		{
			VE_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}
}

