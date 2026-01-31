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
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		/*ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}*/

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		//ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(VE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(VE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(VE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(VE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(VE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(VE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(VE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(VE_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
		
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey key = VEKeyToImGuiKey(e.GetKeyCode());
		io.AddKeyEvent(key, true);
		// 3. Handle Modifiers (The "Magic" part)
// You don't need to check Left/Right control manually anymore!
		io.AddKeyEvent(ImGuiMod_Ctrl, ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl));
		io.AddKeyEvent(ImGuiMod_Shift, ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift));
		io.AddKeyEvent(ImGuiMod_Alt, ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt));
		io.AddKeyEvent(ImGuiMod_Super, ImGui::IsKeyDown(ImGuiKey_LeftSuper) || ImGui::IsKeyDown(ImGuiKey_RightSuper));
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey key = VEKeyToImGuiKey(e.GetKeyCode());
		io.AddKeyEvent(key, false);

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}