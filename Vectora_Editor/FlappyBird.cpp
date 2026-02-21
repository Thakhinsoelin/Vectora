#include "FlappyBird.hpp"
#include <imgui.h>

FlappyBird::FlappyBird()
:Layer("Flappybird"), m_CameraController(600.f / 600.f)
{
	
}

void FlappyBird::OnAttach()
{
}

void FlappyBird::OnDetach()
{
}

void FlappyBird::OnUpdate(Vectora::Timestep ts)
{
}

void FlappyBird::OnImGuiRender()
{
}

void FlappyBird::OnEvent(Vectora::Event& e)
{
}

BackGround::BackGround()
	: Layer("background"), m_CameraController(600.0f / 600.0f)
{
	
}

void BackGround::OnAttach()
{
	m_BackgroundTexture = Vectora::Texture2D::Create("assets/flappy/Background.png");
	m_GroundTexture = Vectora::Texture2D::Create("assets/flappy/Ground.png");
	
}

void BackGround::OnDetach()
{
}

void BackGround::OnUpdate(Vectora::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	fps = 1 / ts.GetSeconds();

	glm::vec3 test = { 0.f, 0.f, .1f };
	glm::vec2 siz = { 0.f, 0.f };
	ScreenUtils::GetOpenGLTransform(0, 600 - 256, 256, 256, 600, 600, test, siz);
	Vectora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Vectora::RenderCommand::Clear();

	Vectora::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Vectora::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Vectora::Renderer2D::DrawQuad(pos, { 2.0f, 2.0f }, m_BackgroundTexture);
	Vectora::Renderer2D::DrawQuad({ pos.x + 2.f, 0.0f }, { 2.0f, 2.0f }, m_BackgroundTexture);
//	Vectora::Renderer2D::DrawQuad({ -1.f, -1.0f, 0.1f }, { 1.0f, 1.0f }, m_GroundTexture);
	Vectora::Renderer2D::DrawQuad(test, siz, m_GroundTexture);
	pos.x -= backgroundSpeed * ts;
	if (pos.x <= -2.f)
	{
		VE_TRACE("pos resetted");
		pos.x = 0.f;
	}
	Vectora::Renderer2D::EndScene();
}

void BackGround::OnImGuiRender()
{
	ImGui::Begin("Utils");
	ImGui::Text("Fps: %f", fps);
	ImGui::SliderFloat("background speed", &backgroundSpeed, 0.f, 2.f);

	ImGui::End();
}

void BackGround::OnEvent(Vectora::Event& e)
{
	m_CameraController.OnEvent(e);
}
