#pragma once
#include "Core.h"
#include "vpch.h"
#include "Events/Event.h"

namespace Vectora {
	class VECTORA_API Layer
	{
	public:
		Layer(const std::string& debugname = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event)
		{
		};

		inline const std::string& GetName() const { return m_Debugname; }
	private:
		std::string m_Debugname;
	};
}

