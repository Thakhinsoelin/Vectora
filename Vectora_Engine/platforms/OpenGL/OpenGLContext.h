#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Vectora {
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override final;
		virtual void SwapBuffers() override final;
	private:
		GLFWwindow* m_WindowHandle;
	};
}
