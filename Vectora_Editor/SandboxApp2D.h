#pragma once

#include "Vectora.h"

class Sandbox2D : public Vectora::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vectora::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vectora::Event& e) override;
private:
	Vectora::OrthographicCameraController m_CameraController;

	// Temp
	Vectora::Ref<Vectora::VertexArray> m_SquareVA;
	Vectora::Ref<Vectora::Shader> m_FlatColorShader;
	Vectora::Ref<Vectora::Texture2D> m_CheckerboardTexture;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};
	float fps;
	std::vector<ProfileResult> m_ProfileResults;
};
