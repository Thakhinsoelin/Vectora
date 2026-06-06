#pragma once

#include "Renderer/EditorCamera.h"
#include "Renderer/Model.h"
#include "Renderer/SkeAnimator.h"

namespace Vectora
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const CameraC& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::mat4& transform, const glm::vec4& color);
		static void EndScene();

		// Core Model Drawing API with optional shader parameters
		static void DrawModel(const glm::mat4& transform, const Ref<Model>& model);

		// Advanced variation to draw a model manually using custom overrides textures
		static void DrawModel(const glm::mat4& transform, const Ref<Model>& model, const Material& overrideMaterial);

		static void DrawAnimatedModel(const glm::mat4& transform, const Ref<Model>& model, SkeAnimator& animator);
	};
}

