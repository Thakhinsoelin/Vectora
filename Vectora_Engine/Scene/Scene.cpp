#include "Scene.hpp"

#include "vpch.h"
#include "Entity.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include "Scene/ScriptableEntity.h"
#include "Scripting/ScriptEngine.h"

#include <box2d/box2d.h>
#include <box2d/base.h>
#include <box2d/collision.h>
#include <box2d/types.h>
#include <box2d/id.h>

#include <glm/glm.hpp>

namespace Vectora {
	static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
		case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		VE_CORE_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}

	static void DoMath(const glm::mat4& transform)
	{

	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{

	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcEntity : view)
				{
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

					auto& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth > 0 ? (uint32_t)other->m_ViewportWidth : 1280;
		newScene->m_ViewportHeight = other->m_ViewportHeight > 0 ? (uint32_t)other->m_ViewportHeight : 720;

		//newScene->OnViewportResize(newScene->m_ViewportWidth, newScene->m_ViewportHeight);

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		newScene->OnViewportResize(newScene->m_ViewportWidth, newScene->m_ViewportHeight);
		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		m_EntityMap[uuid] = entity;

		return entity;
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
		{
			return Entity{ m_EntityMap.at(uuid), this};
		}
		return {};
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.GetUUID());
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;
		OnPhysics2DStart();
		{
			ScriptEngine::OnRuntimeStart(this);
			auto view = m_Registry.view<ScriptComponent>();
			for (auto& e : view) {
				Entity entity = { e, this };
				ScriptEngine::OnCreateEntity(entity);

				
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;
		OnPhysics2DStop();
		{
			ScriptEngine::OnRuntimeStop();

			
		}
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{	
		// Scripting 
		
		{
			// C# Entity OnUpdate
			auto view = m_Registry.view<ScriptComponent>();
			for (auto& e : view) {
				Entity entity = { e, this };
				ScriptEngine::OnUpdateEntity(entity, ts);

			}
		

			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {

				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };

					nsc.Instance->OnCreate();
				}

			nsc.Instance->OnUpdate(ts);
			});
		}

		// Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			const int32_t subStepCount = 4;
			b2World_Step(m_PhysicsWorld, ts, subStepCount);

			// Retrieve transform from Box2D
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				//b2Body* body = (b2Body*)rb2d.RuntimeBody;
				b2Vec2 position = b2Body_GetPosition(rb2d.RuntimeBody);
				b2Rot rotation = b2Body_GetRotation(rb2d.RuntimeBody);
				float angle = b2Rot_GetAngle(rotation);

				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = angle;
			}
		}

		CameraC* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			
			{
				auto view = m_Registry.view<CameraComponent>(); // Only view cameras
				for (auto entity : view)
				{
					auto& camera = view.get<CameraComponent>(entity);
					if (camera.Primary)
					{
						if(m_Registry.all_of<TransformComponent>(entity))
						// Only fetch the transform if we actually found the primary camera
						{
							auto& transform = m_Registry.get<TransformComponent>(entity);
							mainCamera = &camera.camera;
							cameraTransform = glm::inverse(transform.GetTransform());
						}
						break;
					}
				}
			}

			if (mainCamera)
			{
				Renderer2D::BeginScene(*mainCamera, cameraTransform);

				{
					
					auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
					view.each([](auto entity, auto& transform, auto& sprite) {
						Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
						});
				}
				{
					
					auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
					// The 'each' method is the preferred modern EnTT way
					view.each([](auto entity, auto& transform, auto& circle) {
						
						Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
						});
				}
				Renderer2D::EndScene();
			}
			/*else {
				VE_CORE_CRITICAL("No camera found");
			}*/
		}


	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
		// Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			const int32_t subStepCount = 4;
			b2World_Step(m_PhysicsWorld, ts, subStepCount);

			// Retrieve transform from Box2D
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				//b2Body* body = (b2Body*)rb2d.RuntimeBody;
				b2Vec2 position = b2Body_GetPosition(rb2d.RuntimeBody);
				b2Rot rotation = b2Body_GetRotation(rb2d.RuntimeBody);
				float angle = b2Rot_GetAngle(rotation);

				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = angle;
			}
		}

		// Render
		RenderScene(camera);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		RenderScene(camera);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.camera.SetViewportSize(width, height);
		}


	}
	
	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		VE_CORE_TRACE("No primary Camera");
		return {};
	}

	void Scene::OnPhysics2DStart()
	{
		b2Vec2 gravity = { 0.f, -9.8f };
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = gravity;
		m_PhysicsWorld = b2CreateWorld(&worldDef);


		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position = { transform.Translation.x, transform.Translation.y };
			bodyDef.rotation = b2MakeRot(transform.Rotation.z);
			//bodyDef.angle = transform.Rotation.z;

			b2BodyId body = b2CreateBody(m_PhysicsWorld, &bodyDef);
			b2Body_SetFixedRotation(body, rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				// 1. Create the Shape Definition (Replaces b2FixtureDef)
				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = bc2d.Density;
				shapeDef.material.friction = bc2d.Friction;
				shapeDef.material.restitution = bc2d.Restitution;
				worldDef.restitutionThreshold = bc2d.RestitutionThreshold;

				b2Polygon boxShape = b2MakeBox(bc2d.Size.x * transform.Scale.x,
					bc2d.Size.y * transform.Scale.y);


				b2ShapeId shapeId = b2CreatePolygonShape(rb2d.RuntimeBody, &shapeDef, &boxShape);


				bc2d.RuntimeFixture = shapeId;
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2Circle circleShape;
				circleShape.radius = transform.Scale.x * cc2d.Radius;
				circleShape.center = { cc2d.Offset.x, cc2d.Offset.y };

				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = cc2d.Density;
				shapeDef.material.friction = cc2d.Friction;
				shapeDef.material.restitution = cc2d.Restitution;
				worldDef.restitutionThreshold = cc2d.RestitutionThreshold;

				b2ShapeId cir = b2CreateCircleShape(body, &shapeDef, &circleShape);

				cc2d.RuntimeFixture = cir;
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		b2DestroyWorld(m_PhysicsWorld);
		m_PhysicsWorld = b2_nullWorldId;
	}

	void Scene::RenderScene(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
		view.each([](auto entity, auto& transform, auto& circle) {
			Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
			});

		Renderer2D::EndScene();
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		/*static_assert(false);*/
		static_assert(sizeof(T) == 0, "This component type does not have a specialized OnComponentAdded implementation!");
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}
}