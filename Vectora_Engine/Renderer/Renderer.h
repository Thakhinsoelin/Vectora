#pragma once
#include "RenderCommand.h"
namespace Vectora {

	class Renderer
	{
	public:
		static void BeginScence();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
	class λ {

	};


}