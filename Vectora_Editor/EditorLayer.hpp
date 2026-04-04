#pragma once

#include "Vectora.h"
#include "Renderer/EditorCamera.h"

#include "Panels/SceneHirearchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		//void OpenScene(const std::string& filepath);
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		// UI Panels
		void UI_Toolbar();

	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Framebuffer> m_FrameBuffer;

		Ref<Texture2D> m_IconPlay;
		Ref<Texture2D> m_IconStop;

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

		EditorCamera m_EditorCamera;

		Entity m_CameraEntity;
		Entity m_SecondCamera;
		Entity m_HoveredEntity;

		glm::vec2 m_ViewportBounds[2];
		int m_GizmoType = -1;

		bool m_PrimaryCamera = true;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		SceneHirearchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		enum class SceneState {
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;
	};
}
