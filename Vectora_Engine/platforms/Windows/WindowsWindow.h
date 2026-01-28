#pragma once

#include "Window.h"
#include "platforms/Windows/WindowsWindow.h"
#include <GLFW/glfw3.h>

namespace Vectora {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& prop);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		inline VE_UINT GetWidth() const override { return m_Data.Width; }
		inline VE_UINT GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override {
			this->m_Data.EventCallback = callback;
		}
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& prop);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

