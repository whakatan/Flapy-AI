#include "pch.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Entities/BehaviorBatch.h"
#include "Scenes/Entities/Behavior.h"
#include <mutex>

namespace SoulEngine
{
	BehaviorBatch::BehaviorBatch(Scene* scene) : scene(scene) {}

	BehaviorBatch::~BehaviorBatch()
	{
		for (auto behavior : behaviors)
			if (behavior->_enabled)
				behavior->OnDestroy();

		entities.clear();
		behaviors.clear();
	}

	bool BehaviorBatch::Contains(GameObject gameObject)
	{
		entt::entity entity = gameObject;
		return std::find(entities.begin(), entities.end(), entity) != entities.end();
	}

	void BehaviorBatch::EmplaceBack(Behavior* behavior, Scene* scene, entt::entity entity)
	{
		auto it = std::find(entities.begin(), entities.end(), entity);
		if (it == entities.end()) entities.emplace_back(entity);

		behaviors.emplace_back(behavior);
		behavior->_gameObject = { scene, entity };
		behavior->Create();
	}

	void BehaviorBatch::Destroy(GameObject gameObject)
	{
		if (!Contains(gameObject)) return;

		entt::entity entity = gameObject;

		entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](auto e) { return entity == e; }), entities.end());
		behaviors.erase(std::remove_if(behaviors.begin(), behaviors.end(), [&gameObject](auto behavior) { if (gameObject == behavior->_gameObject) { behavior->OnDestroy();  return true; } return false; }), behaviors.end());
	}

	void BehaviorBatch::Remove(Behavior* behavior)
	{
		behavior->_enabled = false;
		entt::entity entity = behavior->_gameObject;

		auto start = behaviors.begin();
		auto end = behaviors.end();
		for (; start != end; ++start)
		{
			if (*start._Ptr == behavior)
			{
				behavior->OnDestroy();
				behavior->_enabled = false;
				behaviors.erase(start);
				break;
			}
		}

		auto it = std::find_if(behaviors.begin(), behaviors.end(), [&](auto b) { return b->_gameObject == behavior->_gameObject; });
		if (it == behaviors.end())
			entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](auto e) { return entity == e; }));
	}

	void BehaviorBatch::Start()
	{
		auto count = behaviors.size();
		for (int i = 0; i < count; ++i)
		{
			auto behavior = behaviors[i];
			if (!behavior->_enabled)
			{
				behavior->Start();
				behavior->_enabled = true;
			}
		}

		if (count != behaviors.size())
			Start();
	}

	void BehaviorBatch::Start(GameObject gameObject)
	{
		if (!Contains(gameObject))
			return;

		for (auto behavior : behaviors)
		{
			if (!behavior->_enabled && behavior->_gameObject == gameObject)
			{
				behavior->Start();
				behavior->_enabled = true;
			}
		}
	}

	void BehaviorBatch::Update()
	{
		auto count = behaviors.size();
		for (int i = 0; i < count; ++i)
		{
			auto behavior = behaviors[i];
			if (behavior->_enabled)
				behavior->Update();
		}
	}

	void BehaviorBatch::FixedUpdate()
	{
		auto count = behaviors.size();
		for (int i = 0; i < count; ++i)
		{
			auto behavior = behaviors[i];
			if (behavior->_enabled)
				behavior->FixedUpdate();
		}
	}

	void BehaviorBatch::LateUpdate()
	{
		auto count = behaviors.size();
		for (int i = 0; i < count; ++i)
		{
			auto behavior = behaviors[i];
			if (behavior->_enabled)
				behavior->LateUpdate();
		}
	}

	void BehaviorBatch::OnCollisionEnter(GameObject gameObject, const Collision2D& collision)
	{
		auto count = behaviors.size();
		for (int i = 0; i < count; ++i)
		{
			auto behavior = behaviors[i];
			if (behavior->_enabled)
				if (behavior->_enabled && gameObject == behavior->_gameObject)
					behavior->OnCollisionEnter(collision);
		}
	}

	void BehaviorBatch::OnCollisionExit(GameObject gameObject, const Collision2D& collision)
	{
		auto count = behaviors.size();
		for (int i = 0; i < count; ++i)
		{
			auto behavior = behaviors[i];
			if (behavior->_enabled)
				if (behavior->_enabled && gameObject == behavior->_gameObject)
					behavior->OnCollisionExit(collision);
		}
	}

	void BehaviorBatch::OnTriggerEnter(GameObject gameObject, const Collision2D& collision)
	{
		auto count = behaviors.size();
		for (int i = 0; i < count; ++i)
		{
			auto behavior = behaviors[i];
			if (behavior->_enabled)
				if (behavior->_enabled && gameObject == behavior->_gameObject)
					behavior->OnTriggerEnter(collision);
		}
	}

	void BehaviorBatch::OnTriggerExit(GameObject gameObject, const Collision2D& collision)
	{
		auto count = behaviors.size();
		for (int i = 0; i < count; ++i)
		{
			auto behavior = behaviors[i];
			if (behavior->_enabled)
				if (behavior->_enabled && gameObject == behavior->_gameObject)
					behavior->OnTriggerExit(collision);
		}
	}
}