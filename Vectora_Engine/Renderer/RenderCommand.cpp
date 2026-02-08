#include "RenderCommand.h"
#include "vpch.h"
#include "platforms/OpenGL/OpenGLRendererAPI.h"

namespace Vectora{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}