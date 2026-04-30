#include "vpch.h"
#include "Core/Log.h"
#include "Core/KeyCodes.h"
#include "Core/Input.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/reflection.h>

#define VE_ADD_INTERNAL_CALLS(Name) mono_add_internal_call("Vectora.InternalCalls::" #Name, (const void*)Name)

namespace Vectora {

	std::unordered_map < MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFunc;


	static void NativeLog(MonoString* text, int param) {
		char* cStr = mono_string_to_utf8(text);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << param << "\n";
	}

	static void NativeLogV3(glm::vec3* parameter) {
		VE_WARN("{0}", *parameter);
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		VE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		VE_CORE_ASSERT(entity);
		if (entity) {
			*outTranslation = entity.GetComponent<TransformComponent>().Translation;
		}
		else {
			VE_CORE_ERROR("Entity with UUID {0} not found!", entityID);
			*outTranslation = glm::vec3(0.0f);
		}
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		VE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		VE_CORE_ASSERT(entity);
		if (entity) {
			entity.GetComponent<TransformComponent>().Translation = *translation;
		}
		else {
			VE_CORE_ERROR("Entity with UUID {0} not found!", entityID);
		}
	}

	static bool Input_IsKeyDown(VE_KEYCODE keyCode) {
		return Input::IsKeyPressed(keyCode);
	}

	

	static bool Entity_HasComponent(UUID iD, MonoReflectionType* componentType) {
		
		Scene* scene = ScriptEngine::GetSceneContext();
		VE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(iD);
		VE_CORE_ASSERT(entity);

		MonoType* managedType =  mono_reflection_type_get_type(componentType);
		VE_CORE_ASSERT(s_EntityHasComponentFunc.find(managedType) != s_EntityHasComponentFunc.end());
		return s_EntityHasComponentFunc.at(managedType)(entity);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID id, glm::vec2* impulse, glm::vec2* point, bool wake) {
		Scene* scene = ScriptEngine::GetSceneContext();
		VE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(id);
		VE_CORE_ASSERT(entity);
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		rb2d.RuntimeBody;
		b2Body_ApplyLinearImpulse(rb2d.RuntimeBody, b2Vec2{ impulse->x, impulse->y }, b2Vec2{ point->x, point->y }, true);

	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID id, glm::vec2* impulse, bool wake) {
		Scene* scene = ScriptEngine::GetSceneContext();
		VE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(id);
		VE_CORE_ASSERT(entity);
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		rb2d.RuntimeBody;

		b2Body_ApplyLinearImpulseToCenter(rb2d.RuntimeBody, b2Vec2{ impulse->x, impulse->y }, true);
	}

	void ScriptGlue::RegisterFunctions()
	{
		VE_ADD_INTERNAL_CALLS(NativeLogV3);
		VE_ADD_INTERNAL_CALLS(TransformComponent_GetTranslation);
		VE_ADD_INTERNAL_CALLS(TransformComponent_SetTranslation);
		VE_ADD_INTERNAL_CALLS(Entity_HasComponent);

		VE_ADD_INTERNAL_CALLS(Rigidbody2DComponent_ApplyLinearImpulse);
		VE_ADD_INTERNAL_CALLS(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
		VE_ADD_INTERNAL_CALLS(Input_IsKeyDown);
	}

	template<typename ... Component>
	static void RegisterComponent() {
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);

			std::string managedTypeName = fmt::format("Vectora.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(),
				ScriptEngine::GetCoreAssemblyImage());
			if (!managedType) {
				VE_CORE_ERROR("Could not find managed type for component {}", managedTypeName);
				return;
			}
			s_EntityHasComponentFunc[managedType] = [](Entity entity) {
				return entity.HasComponent<Component>();
				};

		}(), ...);
		
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component ...>) {
		RegisterComponent<Component ...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});

	}
}