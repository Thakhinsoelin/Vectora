#include "SandboxApp2D.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "platforms/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	VE_PROFILE_FUNCTION();
	m_CheckerboardTexture = Vectora::Texture2D::Create("assets/textures/Checkerboard.png");
}

void test() <%
	3 + 8;
%>

void Sandbox2D::OnDetach()
{
	VE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Vectora::Timestep ts)
{
	fps = 1 / ts.GetSeconds();
	VE_PROFILE_FUNCTION();
	// Update
	{
		//VE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	Vectora::Renderer2D::ResetStats();
	{
		VE_PROFILE_SCOPE("Renderer Prep");
		Vectora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vectora::RenderCommand::Clear();
	}
	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		VE_PROFILE_SCOPE("Renderer Draw");
		Vectora::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		Vectora::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vectora::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vectora::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//Vectora::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, glm::radians(80.f), m_CheckerboardTexture, 10.f, glm::vec4(1.f, .9f, .8f, 1.f));
		Vectora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Vectora::Renderer2D::DrawRotatedQuad({ -2.f, 0.f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
		Vectora::Renderer2D::EndScene();

		Vectora::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Vectora::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Vectora::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	VE_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	auto stats = Vectora::Renderer2D::GetStats();
	ImGui::Text("Frames per second %f: ", fps);
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

	ImGui::Begin("Renderer");
	/*ImGui::Text("Time taken in milliseconds: %f\n", ts);*/
	ImGui::TextColored({ 0.f, 1.0, 0.f, 1.0f }, "Opengl info\n");
	ImGui::TextColored({ 0.f, 1.0, 0.f, 1.0f }, "Vendor: %s\n", (const char*)glGetString(GL_VENDOR));
	ImGui::TextColored({ 0.f, 1.0, 0.f, 1.0f }, "Renderer: %s\n", (const char*)glGetString(GL_RENDERER));
	ImGui::TextColored({ 0.f, 1.0, 0.f, 1.0f }, "Version: %s\n", (const char*)glGetString(GL_VERSION));
	ImGui::End();
}

void Sandbox2D::OnEvent(Vectora::Event& e)
{
	m_CameraController.OnEvent(e);
}