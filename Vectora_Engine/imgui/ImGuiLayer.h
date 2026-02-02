#pragma once
#include "Core.h"
#include "Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
// FORWARD DECLARATION: This tells the compiler "ImGuiContext is a struct."
struct ImGuiContext;
namespace Vectora {

	class VECTORA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override final;
		virtual void OnDetach() override final;
		virtual void OnImGuiRender() override final;

		void Begin();
		void End();
		::ImGuiContext* GetContext() { return m_Context; }
		static ImGuiLayer* GetImguiLayerInstance() { return s_ImGuiLayerInstance; }
	private:
		float m_Time = 0.0f;
		::ImGuiContext* m_Context;
		static ImGuiLayer* s_ImGuiLayerInstance;
	};

}