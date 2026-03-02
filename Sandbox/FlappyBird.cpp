#include "FlappyBird.hpp"
#include <imgui.h>
#include <random>
std::vector<Pipe> m_Pipes;
enum class GameState { Start, Playing, GameOver };
static GameState s_GameState = GameState::Start;

// Simple struct to share bird info with the TileLayer for collisions
struct SharedData {
	float BirdY;
	float BirdWidth = 50.0f;
	float BirdX = (600.0f / 2.0f) - 25.0f;
};
static SharedData s_Data;

FlappyBird::FlappyBird()
	:Layer("Flappybird"), m_Camera(0, 600, 600, 0)
{
	m_BirdTexture = Vectora::Texture2D::Create("assets/flappy/Bird.png");
}
void DrawQuadTopLeft(glm::vec3 pos, glm::vec2 size, const Vectora::Ref<Vectora::Texture2D>& texture, float rotation)
{
	// Math: CenterPosition = TopLeftPosition + (Size / 2)
	glm::vec3 actualPos = { pos.x + (size.x / 2.0f), pos.y + (size.y / 2.0f), pos.z };
	Vectora::Renderer2D::DrawRotatedQuad(actualPos, {-size.x, size.y}, rotation, texture);
}

bool CheckCollision(float birdX, float birdY, float birdSize, const Pipe& pipe, float pipeWidth, float gapY, float gapSize) {
	// 1. Find the edges of the Bird
	float birdRight = birdX + birdSize;
	float birdBottom = birdY + birdSize;

	// 2. Find the edges of the Pipe
	float pipeRight = pipe.Position.x + pipeWidth;

	// 3. Check if we are horizontally overlapping with the pipe
	bool intersectsX = birdRight > pipe.Position.x && birdX < pipeRight;

	if (intersectsX) {
		// 4. Check if we hit the Top Pipe (Bird's top is above the gap's top)
		if (birdY < pipe.GapY)
			return true;

		// 5. Check if we hit the Bottom Pipe (Bird's bottom is below the gap's bottom)
		if (birdBottom > pipe.GapY + gapSize)
			return true;
	}

	return false;
}

void FlappyBird::OnAttach()
{
}

void FlappyBird::OnDetach()
{
}

void FlappyBird::OnUpdate(Vectora::Timestep ts)
{
	float dt = ts.GetSeconds();
	float targetRotation = m_BirdVelocityY * 0.15f;

	// 2. Clamp it so it doesn't spin like a propeller
	m_BirdRotation = std::clamp(targetRotation, -45.0f, 90.0f);
	// 1. Input - The "Flap"
	if (Vectora::Input::IsKeyPressed(VE_KEY_SPACE)) {
		m_BirdVelocityY = m_FlapForce;
	}

	// 2. Physics - Gravity & Speed Limits
	m_BirdVelocityY += m_Gravity * dt;

	if (m_BirdVelocityY > m_TerminalVelocity)
		m_BirdVelocityY = m_TerminalVelocity;

	if (m_BirdY < 0) { // Ceiling
		m_BirdY = 0;
	}

	if (m_BirdY > 550) { // Ground (assuming 600 is bottom and ground is 50px tall)
		// Here is where you would trigger "GameOver()"
		m_BirdY = 550;
	}
	// 3. Movement - Update Position
	m_BirdY += m_BirdVelocityY * dt;
	float birdWidth = 50.0f;
	float birdHeight = 50.0f;

	for (auto& pipe : m_Pipes) {
		if (CheckCollision((600.f / 2) - birdWidth / 2, m_BirdY, birdWidth, pipe, 60.f, pipe.GapY, 200.f)) {
			VE_TRACE("Collision Detected!");
			// Here is where you would trigger "GameOver()"
		};
	}

	Vectora::Renderer2D::BeginScene(m_Camera);

	//Vectora::Renderer2D::DrawQuad({ birdWidth/2, birdHeight/2, 1.f }, { birdWidth, birdHeight }, m_BirdTexture);
	DrawQuadTopLeft({ (600.f / 2) - birdWidth / 2, /*((600.f / 2) - birdHeight / 2)+*/ m_BirdY, 0.3f }, { birdWidth, birdHeight }, m_BirdTexture, 180.f + m_BirdRotation);
	Vectora::Renderer2D::EndScene();
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
	// You'll need the native GLFW window handle
	GLFWwindow* window = (GLFWwindow*)Vectora::Application::Get().GetWindow().GetNativeWindow();
	glfwSetWindowSizeLimits(window, 600, 600, 600, 600); // Min and Max are the same
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

	Vectora::Renderer2D::DrawQuad({ pos.x, bgY, 0.0f }, { 2.0f, bgHeight }, m_BackgroundTexture);
	Vectora::Renderer2D::DrawQuad({ pos.x + 2.f, bgY, 0.0f }, { 2.0f, bgHeight }, m_BackgroundTexture);
	Vectora::Renderer2D::DrawQuad({ -1.f, groundY, 0.2f }, { 1.f, groundHeight }, m_GroundTexture);
	Vectora::Renderer2D::DrawQuad({ 0.f, groundY, 0.2f }, { 1.f, groundHeight }, m_GroundTexture);
	Vectora::Renderer2D::DrawQuad({ 1.f, groundY, 0.2f }, { 1.f, groundHeight }, m_GroundTexture);
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
	: Layer("background"), m_Camera(0.f, 600.f, 600.f, 0)
{

}

void TileLayer::OnAttach()
{
	m_TileTexture = Vectora::Texture2D::Create("assets/flappy/Tile.png");
	// Initialize the pipes
	// In OnAttach, initialize with some spacing
	float xStart = 600.0f; // Start at the right edge of the screen
	for (int i = 0; i < 5; i++) {
		m_Pipes.push_back({ { xStart + (i * m_PipeSpacing), 0.0f }, 200.0f });
	}

}

void TileLayer::OnDetach()
{
}

void TileLayer::OnUpdate(Vectora::Timestep ts)
{
	static std::random_device rd;
	static std::mt19937 engine(rd());

	// Range for GapY: Keep the gap between 50px and 400px so it's always on screen
	static std::uniform_real_distribution<float> dist(50.0f, 400.0f);

	float m_PipeWidth = 60.0f;
	float m_PipeHeight = 600.0f;
	float m_GapSize = 200.0f;

	Vectora::Renderer2D::BeginScene(m_Camera);

	for (auto& pipe : m_Pipes) {
		// 1. Move
		pipe.Position.x -= tileSpeed * ts.GetSeconds();

		// 2. Reset if off-screen (600 is your window width)
		if (pipe.Position.x < -100.f) {
			pipe.Position.x += m_Pipes.size() * m_PipeSpacing;
			pipe.GapY = dist(engine);
		}

		// 3. DRAW TOP PIPE
		// The bottom of this pipe needs to be at pipe.GapY.
		// So the Top-Left Y must be GapY minus the pipe's height.
		DrawQuadTopLeft(
			{ pipe.Position.x, pipe.GapY - m_PipeHeight, 0.1f },
			{ m_PipeWidth, m_PipeHeight },
			m_TileTexture, 180.f
		);

		// 4. DRAW BOTTOM PIPE
		// The top of this pipe needs to be at pipe.GapY + GapSize.
		DrawQuadTopLeft(
			{ pipe.Position.x, pipe.GapY + m_GapSize, 0.1f },
			{ m_PipeWidth, m_PipeHeight },
			m_TileTexture, 180.f
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
