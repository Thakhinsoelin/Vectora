#include "FlappyBird.hpp"
#include <imgui.h>
#include <random>

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
	float groundHeight = 0.3f;
	float bgHeight = 2.0f - groundHeight; // Result: 1.6f

	// 1. Position the Background at the TOP
	// The top of the screen is 1.0. To align the top of the texture with the top of the screen:
	// Y = TopLimit - (Height / 2)
	float bgY = 1.0f - (bgHeight / 2.0f);

	// 2. Position the Ground at the BOTTOM
	// The bottom of the screen is -1.0. To align the bottom of the texture with the bottom of the screen:
	// Y = BottomLimit + (Height / 2)
	float groundY = -1.0f + (groundHeight / 2.0f);
	//ScreenUtils::GetOpenGLTransform(0, 600 - 256, 256, 256, 600, 600, test, siz);
	Vectora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Vectora::RenderCommand::Clear();

	Vectora::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Vectora::Renderer2D::DrawQuad({ pos.x, bgY }, { 2.0f, bgHeight }, m_BackgroundTexture);
	Vectora::Renderer2D::DrawQuad({ pos.x + 2.f, bgY }, { 2.0f, bgHeight }, m_BackgroundTexture);
	Vectora::Renderer2D::DrawQuad({ -1.f, groundY, 0.1f }, { 1.f, groundHeight }, m_GroundTexture);
	Vectora::Renderer2D::DrawQuad({ 0.f, groundY, 0.1f }, { 1.f, groundHeight }, m_GroundTexture);
	Vectora::Renderer2D::DrawQuad({ 1.f, groundY, 0.1f }, { 1.f, groundHeight }, m_GroundTexture);
	//Vectora::Renderer2D::DrawQuad(test, siz, m_GroundTexture);
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
	//Vectora::EventDispatcher dispatcher(e);
	//// Listen for the WindowResizeEvent
	//dispatcher.Dispatch<Vectora::WindowResizeEvent>(VE_BIND_EVENT_FN(BackGround::OnWindowResize));
}

bool BackGround::OnWindowResize(Vectora::WindowResizeEvent& e)
{
	//glViewport(0, 0, e.GetWidth(), e.GetHeight());
	float aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	m_CameraController.GetCamera().SetProjection(-aspectRatio * 1, aspectRatio * 1, -1, 1);
	return false;
}

TileLayer::TileLayer()
	: Layer("background"), m_CameraController(600.0f / 600.0f)
{

}

void TileLayer::OnAttach()
{
	m_TileTexture = Vectora::Texture2D::Create("assets/flappy/Tile.png");
	// Initialize the pipes
	int pipeCount = 5;
	float m_PipeSpacing = 1.5f; // Horizontal distance between pipes

	for (int i = 0; i < pipeCount; i++)
	{
		Pipe pipe;
		// Start the first pipe at X=2.0f, second at 3.5f, etc.
		pipe.Position = { 2.0f + (i * m_PipeSpacing), 0.0f };
		pipe.GapY = 0.0f; // Or use your random logic here
		m_Pipes.push_back(pipe);
	}

}

void TileLayer::OnDetach()
{
}

void TileLayer::OnUpdate(Vectora::Timestep ts)
{
	// 1. Obtain a seed for the random number engine
	static std::random_device rd;

	// 2. Standard mersenne_twister_engine seeded with rd()
	static std::mt19937 engine(rd());

	// 3. Define the distribution for floats in a specific range [min, max)
	// Example: range [0.0f, 10.0f)
	static std::uniform_real_distribution<float> dist(-0.5f, 0.5f);
	float random_number = dist(engine);
	Vectora::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (auto& pipe : m_Pipes) {
		pipe.Position.x -= tileSpeed * ts.GetSeconds();
	}
	/*Vectora::Renderer2D::DrawQuad({ 0.f, 0.f, 1.f }, { 1.f, 1.f }, { 1.0f, 0.f, 0.f, 1.f });
	Vectora::Renderer2D::DrawQuad(
		{ 0.f, 0.f, 1.f },
		{ 0.3f, 2.0f },
		m_TileTexture
	);*/
	// Draw Top Pipe
	for (auto& pipe : m_Pipes) {
		if (pipe.Position.x < -2.5f) { // Off-screen left
			pipe.Position.x += m_Pipes.size() * m_PipeSpacing;
			pipe.GapY = random_number; // Change the height randomly
		}

		Vectora::Renderer2D::DrawQuad(
			{ pipe.Position.x, pipe.GapY + 1.2f, 1.f },
			{ 0.3f, 2.0f },
			m_TileTexture
		);

		// Draw Bottom Pipe
		Vectora::Renderer2D::DrawQuad(
			{ pipe.Position.x, pipe.GapY - 1.2f, 1.f },
			{ 0.3f, 2.0f },
			m_TileTexture
		);
	}
	

	Vectora::Renderer2D::EndScene();

}

void TileLayer::OnImGuiRender()
{
}

void TileLayer::OnEvent(Vectora::Event& e)
{
}
