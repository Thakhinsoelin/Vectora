#pragma once
#include "Renderer/RendererAPI.h"

namespace Vectora {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Clear() override final;
		virtual void SetClearColor(const glm::vec4& color) override final;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override final;
	};
}