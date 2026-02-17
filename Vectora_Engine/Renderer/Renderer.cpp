#include "Renderer.h"
#include "Renderer2D.h"
#include "vpch.h"
#include "platforms/OpenGL/OpenGLShader.h"

namespace Vectora {
	Scope<Renderer::SceneData> Renderer::sceneData = CreateScope<Renderer::SceneData>();
	void Renderer::BeginScence(OrthoGraphicCamera& camera)
	{
		sceneData->ViewProjectionMatrix = camera.GetPV();
	}

	void Renderer::Init() {
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown() {
		
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::EndScene()
	{

	}
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4("u_ViewProjection", sceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
