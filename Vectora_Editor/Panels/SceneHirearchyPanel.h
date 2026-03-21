#pragma once

#include "Core/Core.h"
#include "Scene/Scene.hpp"
#include "Scene/Entity.h"

namespace Vectora {
	class SceneHirearchyPanel {
	public:
		SceneHirearchyPanel() = default;
		SceneHirearchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
		Entity GetSelectedEntity() const { return m_SelectionContext; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}