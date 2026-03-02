#include "Vectora.h"
#include "platforms/OpenGL/OpenGLShader.h"

#include <imgui.h>
#include <imgui/ImGuiLayer.h>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "EditorLayer.hpp"

//-----------Entry Point------------//
#include "Core/EntryPoint.h"
//----------------------------------//
namespace Vectora {
    class SandBox : public Application {
    public:
        SandBox() : Application("Vectora Editor"){
            PushLayer(new EditorLayer());
            // SYNC CONTEXT: This prevents the Segfault.
            // Only turns this on if you were building the core as a dll and linking to your app dynamically.
            /*auto* imguiLayer = (Vectora::ImGuiLayer*)Vectora::ImGuiLayer::GetImguiLayerInstance();
            ImGui::SetCurrentContext(imguiLayer->GetContext());*/
        }
        ~SandBox() {

        }
    };

    Application* CreateApplication() {
        return new SandBox();
    }
}
