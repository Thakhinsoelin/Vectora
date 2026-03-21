#include "vpch.h"
#include "ImGuiLayer.h"
#include "Debug/Instrumentor.h"
#include "Core/Application.h"
#include "Core/KeyCodes.h"
#include "Math/Math.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Vectora {
	ImGuiLayer* ImGuiLayer::s_ImGuiLayerInstance;
	static ImGuiKey VEKeyToImGuiKey(Key key)
	{
		switch (key)
		{
		case Key::VE_KEY_TAB:           return ImGuiKey_Tab;
		case Key::VE_KEY_LEFT:          return ImGuiKey_LeftArrow;
		case Key::VE_KEY_RIGHT:         return ImGuiKey_RightArrow;
		case Key::VE_KEY_UP:            return ImGuiKey_UpArrow;
		case Key::VE_KEY_DOWN:          return ImGuiKey_DownArrow;
		case Key::VE_KEY_PAGE_UP:       return ImGuiKey_PageUp;
		case Key::VE_KEY_PAGE_DOWN:     return ImGuiKey_PageDown;
		case Key::VE_KEY_HOME:          return ImGuiKey_Home;
		case Key::VE_KEY_END:           return ImGuiKey_End;
		case Key::VE_KEY_INSERT:        return ImGuiKey_Insert;
		case Key::VE_KEY_DELETE:        return ImGuiKey_Delete;
		case Key::VE_KEY_BACKSPACE:     return ImGuiKey_Backspace;
		case Key::VE_KEY_SPACE:         return ImGuiKey_Space;
		case Key::VE_KEY_ENTER:         return ImGuiKey_Enter;
		case Key::VE_KEY_ESCAPE:        return ImGuiKey_Escape;
		case Key::VE_KEY_A:             return ImGuiKey_A;
		case Key::VE_KEY_C:             return ImGuiKey_C;
		case Key::VE_KEY_V:             return ImGuiKey_V;
		case Key::VE_KEY_X:             return ImGuiKey_X;
		case Key::VE_KEY_Y:             return ImGuiKey_Y;
		case Key::VE_KEY_Z:             return ImGuiKey_Z;
		case Key::VE_KEY_LEFT_CONTROL:  return ImGuiKey_LeftCtrl;
		case Key::VE_KEY_LEFT_SHIFT:    return ImGuiKey_LeftShift;
		case Key::VE_KEY_LEFT_ALT:      return ImGuiKey_LeftAlt;
		case Key::VE_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
		case Key::VE_KEY_RIGHT_SHIFT:   return ImGuiKey_RightShift;
		case Key::VE_KEY_RIGHT_ALT:     return ImGuiKey_RightAlt;
			// ... add more as needed
		default:                   return ImGuiKey_None;
		}
	}

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
		s_ImGuiLayerInstance = this;
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if(m_BlockEvents){
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::OnAttach()
	{
		VE_PROFILE_FUNCTION();
		IMGUI_CHECKVERSION();
		m_Context = ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);


		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

	}

	void ImGuiLayer::OnDetach()
	{
		VE_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		VE_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();


	}

	void ImGuiLayer::End()
	{
		VE_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
}