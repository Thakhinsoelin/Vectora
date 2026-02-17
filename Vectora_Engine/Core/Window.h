#pragma once
#include "vpch.h"
#include "Core.h"
#include "Events/Event.h"

namespace Vectora {
	struct WindowProps {
		int width;
		int height;
		const char* title;
		WindowProps(const char* title = "Vectora Engine",
			int width = 1280,
			int height = 720)
			: title(title), width(width), height(height) {
		}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {};
		virtual void OnUpdate() = 0;
		virtual VE_UINT GetWidth() const = 0;
		virtual VE_UINT GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;
		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		static Scope<Window> Create(const WindowProps& props = WindowProps());
		
	};
}
