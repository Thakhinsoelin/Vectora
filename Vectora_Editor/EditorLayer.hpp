#pragma once

#include "Vectora.h"
#include "Panels/SceneHirearchyPanel.h"

namespace Vectora
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Framebuffer> m_FrameBuffer;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};
		float fps = 0.f;
		std::vector<ProfileResult> m_ProfileResults;
		glm::vec2 m_ViewportSize = { 0.f, 0.f };

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;

		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		SceneHirearchyPanel m_SceneHierarchyPanel;
	};
}
