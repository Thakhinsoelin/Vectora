#include "vpch.h"
#include "Vectora.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#define VE_ADD_INTERNAL_CALLS(Name) mono_add_internal_call("Vectora.InternalCalls::" #Name, (const void*)Name)

namespace Vectora {
	static void NativeLog(MonoString* text, int param) {
		char* cStr = mono_string_to_utf8(text);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << param << "\n";
	}

	static void NativeLogV3(glm::vec3* parameter) {
		VE_WARN("{0}", *parameter);
	}

	static void Entity_GetTranslation(UUID entityID, glm::vec3* outTranslation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		if (entity) {
			*outTranslation = entity.GetComponent<TransformComponent>().Translation;
		}
		else {
			VE_CORE_ERROR("Entity with UUID {0} not found!", entityID);
			*outTranslation = glm::vec3(0.0f);
		}
	}

	static void Entity_SetTranslation(UUID entityID, glm::vec3* translation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);
		if (entity) {
			entity.GetComponent<TransformComponent>().Translation = *translation;
		}
		else {
			VE_CORE_ERROR("Entity with UUID {0} not found!", entityID);
		}
	}

	void ScriptGlue::RegisterFunctions()
	{
		VE_ADD_INTERNAL_CALLS(NativeLog);
		VE_ADD_INTERNAL_CALLS(NativeLogV3);
		VE_ADD_INTERNAL_CALLS(Entity_GetTranslation);
		VE_ADD_INTERNAL_CALLS(Entity_SetTranslation);
	}
}