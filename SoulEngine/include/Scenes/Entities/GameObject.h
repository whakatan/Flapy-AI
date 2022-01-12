#pragma once
#include "Scenes/Entities/BehaviorBatch.h"
#include "Scenes/Entities/Colliders.h"
#include "Scenes/Entities/Components.h"
#include "Scenes/Entities/Rigidbody2D.h"
#include "Helpers/Uuid.h"

namespace SoulEngine
{
	class Behavior;

	struct Id 
	{
		Uuid uuid;

		Id() = default;
		Id(uint64_t id) : uuid(id) {}
		Id(const Id&) = default;
	};

	class GameObject
	{
	public:
		GameObject() = default;
		GameObject(const GameObject& gameObject) : GameObject(gameObject._scene, gameObject._entity) { }
		GameObject(Scene* scene, entt::entity entity) : _scene(scene), _entity(entity) {}

		Uuid GetId() { return GetComponent<Id>().uuid; }
		std::string GetTag() { return GetComponent<Tag>().tag; }
		Transform& GetTransform() { return GetComponent<Transform>(); }

		template <typename Component>
		bool HasComponent()
		{
			if (!_scene->_registry.valid(_entity)) return false;
			return _scene->_registry.all_of<Component>(_entity);
		}

		template <typename Component, typename... Args>
		Component& AddComponent(Args&&... args)
		{
			CORE_ASSERT(!HasComponent<Component>(), "GameObject already has a component!");
			return _scene->_registry.emplace<Component>(_entity, std::forward<Args>(args)...);
		}

		template <typename Component>
		Component& GetComponent()
		{
			CORE_ASSERT(HasComponent<Component>(), "GameObject does not have component!");
			return _scene->_registry.get<Component>(_entity);
		}

		template <typename Component, typename std::enable_if<!std::is_base_of<Behavior, Component>::value>::type* = nullptr>
		void RemoveComponent()
		{
			CORE_ASSERT(HasComponent<Component>(), "GameObject does not have component!");
			_scene->_registry.remove<Component>(_entity);
		}

		template <typename Component, typename std::enable_if<std::is_base_of<Rigidbody2D, Component>::value>::type* = nullptr>
		Component& AddComponent()
		{
			CORE_ASSERT(!HasComponent<Component>(), "GameObject already has a component!");
			auto& rigidbody = _scene->_registry.emplace<Component>(_entity);
			static_cast<Rigidbody2D*>(&rigidbody)->transform = &GetTransform();
			return rigidbody;
		}

		template <typename Component, typename std::enable_if<std::is_base_of<Collider2D, Component>::value>::type* = nullptr>
		Component& AddComponent()
		{
			CORE_ASSERT(!HasComponent<Component>(), "GameObject already has a component!");
			auto& collider2D = _scene->_registry.emplace<Component>(_entity);
			static_cast<Collider2D*>(&collider2D)->transform = &GetTransform();
			return collider2D;
		}

		template <typename Component, typename std::enable_if<std::is_base_of<Behavior, Component>::value>::type* = nullptr>
		Component& AddComponent()
		{
			CORE_ASSERT(!HasComponent<Component>(), "GameObject already has a component!");
			auto& behavior = _scene->_registry.emplace<Component>(_entity);
			_scene->_behaviorBatch->EmplaceBack(&behavior, _scene, *this);
			return behavior;
		}

		template <typename Component, typename std::enable_if<std::is_base_of<Behavior, Component>::value>::type* = nullptr>
		void RemoveComponent()
		{
			CORE_ASSERT(HasComponent<Component>(), "GameObject does not have component!");
			_scene->_behaviorBatch->Remove<Component>(_entity);
		}

		template <typename T>
		static void AddComponentWithTag(const std::string& tag)
		{
			auto scene = SceneManager::Active();
			scene->_registry.each([&](auto entity) {
				GameObject gameObject{ scene, entity };
				if (gameObject.HasComponent<Tag>() && gameObject.GetTag() == tag)
					gameObject.AddComponent<T>();
				});
		}

		template <typename T>
		static void AddComponentWithTag(const std::string& tag, std::function<void(GameObject)> func)
		{
			auto scene = SceneManager::Active();
			scene->_registry.each([&](auto entity) {
				GameObject gameObject{ scene, entity };
				if (gameObject.HasComponent<Tag>() && gameObject.GetTag() == tag)
				{
					gameObject.AddComponent<T>();
					func(gameObject);
				}
				});
		}

		template <typename T>
		static void AddComponentWithTag(const std::string& tag, std::function<void(GameObject, T&)> func)
		{
			auto scene = SceneManager::Active();
			scene->_registry.each([&](auto entity) {
				GameObject gameObject{ scene, entity };
				if (gameObject.HasComponent<Tag>() && gameObject.GetTag() == tag)
				{
					auto& component = gameObject.AddComponent<T>();
					func(gameObject, component);
				}
				});
		}

		operator bool() const { return _entity != entt::null; }
		operator entt::entity() { return _entity; }
		operator uint32_t() { return static_cast<uint32_t>(_entity); }
		operator uint64_t() { return static_cast<uint64_t>(_entity); }

		bool operator ==(const GameObject& other)
		{
			return _entity == other._entity && _scene == other._scene;
		}

		bool operator !=(const GameObject& other)
		{
			return !(*this == other);
		}

	private:
		entt::entity _entity{ entt::null };
		Scene* _scene{ nullptr };
	};
}