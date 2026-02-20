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

void Sandbox2D::OnDetach()
{
	VE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Vectora::Timestep ts)
{
	VE_PROFILE_FUNCTION();
	// Update
	{
		//VE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		VE_PROFILE_SCOPE("Renderer Prep");
		Vectora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Vectora::RenderCommand::Clear();
	}
	
	{
		VE_PROFILE_SCOPE("Renderer Draw");
		Vectora::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Vectora::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Vectora::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vectora::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//Vectora::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, glm::radians(80.f), m_CheckerboardTexture, 10.f, glm::vec4(1.f, .9f, .8f, 1.f));
		Vectora::Renderer2D::EndScene();
	}

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Vectora::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Vectora::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	VE_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
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