#include "vpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Application.h"
#include "KeyCodes.h"

namespace Vectora {
	ImGuiLayer* ImGuiLayer::s_ImGuiLayerInstance;
	static ImGuiKey VEKeyToImGuiKey(int key)
	{
		switch (key)
		{
		case VE_KEY_TAB:           return ImGuiKey_Tab;
		case VE_KEY_LEFT:          return ImGuiKey_LeftArrow;
		case VE_KEY_RIGHT:         return ImGuiKey_RightArrow;
		case VE_KEY_UP:            return ImGuiKey_UpArrow;
		case VE_KEY_DOWN:          return ImGuiKey_DownArrow;
		case VE_KEY_PAGE_UP:       return ImGuiKey_PageUp;
		case VE_KEY_PAGE_DOWN:     return ImGuiKey_PageDown;
		case VE_KEY_HOME:          return ImGuiKey_Home;
		case VE_KEY_END:           return ImGuiKey_End;
		case VE_KEY_INSERT:        return ImGuiKey_Insert;
		case VE_KEY_DELETE:        return ImGuiKey_Delete;
		case VE_KEY_BACKSPACE:     return ImGuiKey_Backspace;
		case VE_KEY_SPACE:         return ImGuiKey_Space;
		case VE_KEY_ENTER:         return ImGuiKey_Enter;
		case VE_KEY_ESCAPE:        return ImGuiKey_Escape;
		case VE_KEY_A:             return ImGuiKey_A;
		case VE_KEY_C:             return ImGuiKey_C;
		case VE_KEY_V:             return ImGuiKey_V;
		case VE_KEY_X:             return ImGuiKey_X;
		case VE_KEY_Y:             return ImGuiKey_Y;
		case VE_KEY_Z:             return ImGuiKey_Z;
		case VE_KEY_LEFT_CONTROL:  return ImGuiKey_LeftCtrl;
		case VE_KEY_LEFT_SHIFT:    return ImGuiKey_LeftShift;
		case VE_KEY_LEFT_ALT:      return ImGuiKey_LeftAlt;
		case VE_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
		case VE_KEY_RIGHT_SHIFT:   return ImGuiKey_RightShift;
		case VE_KEY_RIGHT_ALT:     return ImGuiKey_RightAlt;
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

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		m_Context = ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
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

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		/*static bool show = true;
		ImGui::ShowDemoWindow(&show);*/
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


	}

	void ImGuiLayer::End()
	{
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
}