#pragma once
#include "Scenes/Scene.h"

namespace SoulEngine
{
	struct Collision2D;
	class Behavior;

	class BehaviorBatch
	{
		Scene* scene{ nullptr };
		std::vector<entt::entity> entities;
		std::vector<Behavior*> behaviors;

		BehaviorBatch(Scene* scene);
		~BehaviorBatch();

		template <typename T>
		T* Get()
		{
			for (auto& behavior : behaviors)
				if (T* t = dynamic_cast<T*>(behavior))
					return t;
			return nullptr;
		}

		bool Contains(GameObject gameObject);

		template <typename T>
		void Remove(entt::entity entity)
		{
			auto start = behaviors.begin();
			auto end = behaviors.end();
			for (; start != end; ++start)
			{
				if (T* behavior = dynamic_cast<T*>(*start))
				{
					if (behavior->_gameObject != entity) continue;

					behavior->_enabled = false;
					behavior->OnDestroy();
					behaviors.erase(start);

					scene->_registry.remove<T>(entity);
					break;
				}
			}

			auto it = std::find_if(behaviors.begin(), behaviors.end(), [&entity](auto b) { return b->_gameObject == entity; });
			if (it == behaviors.end())
				entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](auto e) { return entity == e; }));
		}

		void EmplaceBack(Behavior* behavior, Scene* scene, entt::entity entity);
		void Destroy(GameObject gameObject);
		void Remove(Behavior* behavior);

		void Start();
		void Start(GameObject gameObject);
		void Update();
		void FixedUpdate();
		void LateUpdate();

		void OnCollisionEnter(GameObject gameObject, const Collision2D& collision);
		void OnCollisionExit(GameObject gameObject, const Collision2D& collision);

		void OnTriggerEnter(GameObject gameObject, const Collision2D& collision);
		void OnTriggerExit(GameObject gameObject, const Collision2D& collision);

		friend class Scene;
		friend class Behavior;
		friend class Physics2D;
		friend class GameObject;
		friend class ContactListener;
	};
}