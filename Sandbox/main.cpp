#include "Vectora.h"
#include "platforms/OpenGL/OpenGLShader.h"

#include <imgui.h>
#include <imgui/ImGuiLayer.h>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "SandboxApp2D.h"
#include "FlappyBird.hpp"

//-----------Entry Point------------//
#include "Core/EntryPoint.h"
//----------------------------------//



class TestLayer : public Vectora::Layer {
public:
    TestLayer()
    : Layer("Example"), m_EditorCamera(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
	{
		m_SceneCamera.SetViewportSize(1280.f, 720.f);
		m_SceneCamera.SetPerspective(glm::radians(45.0f), 0.1f, 1000.0f);
		m_SceneCamera.SetProjectionType(Vectora::SceneCamera::ProjectionType::Perspective);
		

		m_VikingRoomModel = Vectora::CreateRef<Vectora::Model>("assets/vampire/dancing_vampire.dae");

		// 2. Load the standalone texture file
		m_VikingRoomTexture = Vectora::Texture2D::Create("assets/textures/viking_room.png");
		m_Animation = Vectora::CreateRef<Vectora::SkeAnimation>("assets/vampire/dancing_vampire.dae", m_VikingRoomModel.get());
		m_Animator = Vectora::CreateRef<Vectora::SkeAnimator>(m_Animation.get());

		m_Animator->PlayAnimation(m_Animation.get());
		// 3. Package it into your custom material state
		m_VikingRoomMaterial.DiffuseMap = m_VikingRoomTexture;
		m_VikingRoomMaterial.Shininess = 16.0f; // Give the wood a slight bounce
    }

    void OnImGuiRender() override
    {
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
		ImGui::End();

		ImGui::Begin("Renderer");
		ImGui::Text("Time taken in milliseconds: %f\n", ts);
		ImGui::TextColored({0.f, 1.0, 0.f, 1.0f}, "Opengl info\n");
		ImGui::TextColored({ 0.f, 1.0, 0.f, 1.0f }, "Vendor: %s\n", (const char*)glGetString(GL_VENDOR));
		ImGui::TextColored({ 0.f, 1.0, 0.f, 1.0f }, "Renderer: %s\n", (const char*)glGetString(GL_RENDERER));
		ImGui::TextColored({ 0.f, 1.0, 0.f, 1.0f }, "Version: %s\n", (const char*)glGetString(GL_VERSION));
		ImGui::End();

    }
    void OnUpdate(Vectora::Timestep ts) override {
		this->ts = ts.GetMilliseconds();
		m_Animator->UpdateAnimation(this->ts);
		// 1. Simple Camera Controls (Optional: Move around with WASD keys if wanted)
		if (Vectora::Input::IsKeyPressed(Vectora::Key::VE_KEY_A)) m_CameraPosition.x -= 2.0f * ts;
		if (Vectora::Input::IsKeyPressed(Vectora::Key::VE_KEY_D)) m_CameraPosition.x += 2.0f * ts;
		if (Vectora::Input::IsKeyPressed(Vectora::Key::VE_KEY_W)) m_CameraPosition.z -= 2.0f * ts;
		if (Vectora::Input::IsKeyPressed(Vectora::Key::VE_KEY_S)) m_CameraPosition.z += 2.0f * ts;

		m_EditorCamera.OnUpdate(ts);

		Vectora::RenderCommand::SetClearColor({ .2f, .3f, .3f, 1.f });
		Vectora::RenderCommand::Clear();
		// 3. Build Camera Transform & View Matrices
		glm::mat4 cameraTransform = glm::translate(glm::mat4(1.0f), m_CameraPosition);

		// As your note said: inverse it before passing to the renderer!
		glm::mat4 viewMatrix = glm::inverse(cameraTransform);

		// 4. Render Scene
		Vectora::Renderer3D::BeginScene(m_EditorCamera);
		
		// Rotate it so the room sits upright (OBJ models often have Y and Z swapped)
		glm::mat4 roomTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		// DRAW: Pass the override material containing your standalone texture!
		//Vectora::Renderer3D::DrawModel(roomTransform, m_VikingRoomModel);
		Vectora::Renderer3D::DrawAnimatedModel(roomTransform, m_VikingRoomModel, *(m_Animator.get()));
		//m_Animator->PlayAnimation(m_Animation.get());
		Vectora::Renderer3D::EndScene();
    }

	bool OnWindowResize(Vectora::WindowResizeEvent& event)
	{
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
			return false;

		// Push the new resolution down so the perspective aspect ratio recalculates!
		m_SceneCamera.SetViewportSize(event.GetWidth(), event.GetHeight());
		return false;
	}

    void OnEvent(Vectora::Event& event) override {
		Vectora::EventDispatcher dispatch(event);
		dispatch.Dispatch<Vectora::KeyPressedEvent>(VE_BIND_EVENT_FN(TestLayer::OnEscapePressed));
		dispatch.Dispatch<Vectora::WindowResizeEvent>(VE_BIND_EVENT_FN(TestLayer::OnWindowResize));
		
		m_EditorCamera.OnEvent(event);
    }

	bool OnEscapePressed(Vectora::KeyPressedEvent& event) {
		if (event.GetKeyCode() == Vectora::Key::VE_KEY_ESCAPE)
		{
			Vectora::Application::Get().SetRunning(false);
		}
		return true;
	}
private:
	Vectora::EditorCamera m_EditorCamera;
	Vectora::SceneCamera m_SceneCamera;

	Vectora::Ref<Vectora::SkeAnimation> m_Animation;
	Vectora::Ref<Vectora::SkeAnimator> m_Animator;
	Vectora::Ref<Vectora::Model> m_VikingRoomModel;
	Vectora::Ref<Vectora::Texture2D> m_VikingRoomTexture;

	// The explicit override material to hold the standalone texture
	Vectora::Material m_VikingRoomMaterial;

	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 5.0f }; // Start 5 units back along Z

	float ts = 0;
	glm::vec4 m_SquareColor = { 0.3f, 0.2f, 0.8f, 1.f};
};

class SandBox : public Vectora::Application {
public:
    SandBox(const Vectora::ApplicationSpecification& spec)
	: Vectora::Application(spec){
		PushLayer(new TestLayer() );
		
		/*PushLayer(new BackGround());
		PushLayer(new TileLayer());
		PushLayer(new FlappyBird());*/
        // SYNC CONTEXT: This prevents the Segfault.
		// Only turns this on if you were building the core as a dll and linking to your app dynamically.
        /*auto* imguiLayer = (Vectora::ImGuiLayer*)Vectora::ImGuiLayer::GetImguiLayerInstance();
        ImGui::SetCurrentContext(imguiLayer->GetContext());*/
    }
    ~SandBox() {

    }
};

Vectora::Application* Vectora::CreateApplication() {
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "./";
    return new SandBox(spec);
}
