#pragma once

#include "Vectora.h"

class ScreenUtils {
public:
	// Converts Pixel units to OpenGL units (-1 to 1 range)
	// x, y: Top-left coordinate in pixels
	// width, height: size in pixels
	// screenWidth, screenHeight: your window size (600, 600)
	static void GetOpenGLTransform(float x, float y, float width, float height,
		float screenWidth, float screenHeight,
		glm::vec3& outPos, glm::vec2& outSize)
	{
		// 1. Calculate the center of the quad in pixel space
		float pixelCenterX = x + (width / 2.0f);
		float pixelCenterY = y + (height / 2.0f);

		// 2. Map Pixels to -1.0 -> 1.0 range
		// X: (pixel / total) * 2 - 1
		// Y: 1 - (pixel / total) * 2  <-- We flip Y because pixels go down, GL goes up
		float glX = (pixelCenterX / screenWidth) * 2.0f - 1.0f;
		float glY = 1.0f - (pixelCenterY / screenHeight) * 2.0f;

		outPos = { glX, glY, 0.0f };

		// 3. Map Size to 0.0 -> 2.0 range
		outSize = { (width / screenWidth) * 2.0f, (height / screenHeight) * 2.0f };
	}
};

class BackGround : public Vectora::Layer {
public:
	BackGround();
	virtual ~BackGround() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vectora::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vectora::Event& e) override;

private:
	bool OnWindowResize(Vectora::WindowResizeEvent& e);

private:
	glm::vec2 pos = { 0.f, 0.f };
	float backgroundSpeed = 0.4f;
	Vectora::OrthographicCameraController m_CameraController;
	Vectora::Ref<Vectora::Texture2D> m_BackgroundTexture;
	Vectora::Ref<Vectora::Texture2D> m_GroundTexture;
	/*float m_LastUpdateEndTime = 0.f;*/
	float fps;
};

struct Pipe {
	glm::vec2 Position;
	float GapY;      // The center point of the gap
	float GapSize;   // How big the opening is
	bool Scored;     // To keep track if the bird passed it
};



class TileLayer : public Vectora::Layer {
public:
	TileLayer();
	virtual ~TileLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vectora::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vectora::Event& e) override;

private:
	glm::vec2 pos = { 0.f, 0.f };
	float tileSpeed = 150.f;
	Vectora::OrthoGraphicCamera m_Camera;
	Vectora::Ref<Vectora::Texture2D> m_TileTexture;

	
	float m_PipeSpawnX = 2.0f; // Start spawning pipes just off-screen
	float m_PipeSpacing = 300.5f; // Horizontal distance between pipes
};

class FlappyBird : public Vectora::Layer
{
public:
	FlappyBird();
	virtual ~FlappyBird() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vectora::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vectora::Event& e) override;
private:
	Vectora::OrthoGraphicCamera m_Camera;
	
	Vectora::Ref<Vectora::Texture2D> m_BirdTexture;
	float m_BirdRotation = 0.f;
	float m_BirdY = 300.0f;          // Position (Middle of screen)
	float m_BirdVelocityY = 0.0f;    // Current speed (up or down)

	// Constants (You'll need to tweak these to "feel" right)
	const float m_Gravity = 1000.0f;   // Acceleration pulling you down
	const float m_FlapForce = -250.0f; // Instant burst of speed UP
	const float m_TerminalVelocity = 400.0f; // Max falling speed (so it doesn't get crazy)

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};
	float fps;
	
	std::vector<ProfileResult> m_ProfileResults;
};

