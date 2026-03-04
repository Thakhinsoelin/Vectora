#include "Entity.h"

#include <vpch.h>

namespace Vectora {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}