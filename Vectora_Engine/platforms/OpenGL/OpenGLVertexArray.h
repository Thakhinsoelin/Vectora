#pragma once

#include "Renderer/VertexArray.h"

namespace Vectora {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override final;
		virtual void Bind() const override final;
		virtual void UnBind() const override final;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override final;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override final;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override final;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override final;
	private:
		VE_UINT32 m_RendererID;
		VE_UINT32 m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}