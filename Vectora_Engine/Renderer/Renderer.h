#pragma once
#include "RenderCommand.h"
#include "OrthographiCamera.h"
#include "Shader.h"

namespace Vectora {

	class VECTORA_API Renderer
	{
	public:
		static void BeginScence(OrthoGraphicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* sceneData;
	};
	class λ {

	};


}