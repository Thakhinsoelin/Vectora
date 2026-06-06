#include "vpch.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Renderer3D.h"
#include "platforms/OpenGL/OpenGLShader.h"
#include "Debug/Instrumentor.h"

namespace Vectora {
	Scope<Renderer::SceneData> Renderer::sceneData = CreateScope<Renderer::SceneData>();
	void Renderer::BeginScence(OrthoGraphicCamera& camera)
	{
		sceneData->ViewProjectionMatrix = camera.GetPV();
	}

	void Renderer::Init() {
		VE_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();
	}

	void Renderer::Shutdown() {
		VE_PROFILE_FUNCTION();
		Renderer3D::Shutdown();
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
