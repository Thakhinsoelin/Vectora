#pragma once

#include "Window.h"
#include "platforms/Windows/WindowsWindow.h"
#include "Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Vectora {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& prop);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override final;
		inline virtual VE_UINT GetWidth() const override final { return m_Data.Width; }
		inline virtual VE_UINT GetHeight() const override final { return m_Data.Height; }

		inline void* GetNativeWindow() const override final { return m_Window; };

		inline void SetEventCallback(const EventCallbackFn& callback) override final {
			this->m_Data.EventCallback = callback;
		}
		virtual void SetVSync(bool enabled) override final;
		virtual bool IsVSync() const override final;
	private:
		virtual void Init(const WindowProps& prop) final;
		virtual void Shutdown() final;

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
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

