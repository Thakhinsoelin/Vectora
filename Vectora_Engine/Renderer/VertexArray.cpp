#include "VertexArray.h"
#include "Renderer.h"
#include "platforms/OpenGL/OpenGlVertexArray.h"

namespace Vectora {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: VE_CORE_ASSERT(false, "RendererApi::None is currently unsupported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		default:
			VE_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
	}
}