#pragma once
#include "Renderer/RendererAPI.h"

namespace Vectora {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Init() override final;
		virtual void Clear() override final;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override final;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override final;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, VE_UINT32 indexCount = 0) override final;
	};
}