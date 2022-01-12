#include "pch.h"
#include "Scenes/Entities/Behavior.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Entities/GameObject.h"

namespace SoulEngine
{
	GameObject Behavior::FindGameObject(const std::string& tag)
	{
		auto scene = SceneManager::Active();

		auto view = scene->_registry.view<Tag>();
		for (auto entity : view)
		{
			auto& tagComponent = view.get<Tag>(entity);
			if (tagComponent.tag == tag)
			{
				return { scene, entity };
			}
		}
		return { scene, entt::null };
	}

	void Behavior::Destroy(GameObject gameObject)
	{
		auto scene = SceneManager::Active();
		if (!scene->_registry.valid(gameObject)) return;

		if (scene->_behaviorBatch->Contains(gameObject))
		{
			for (auto behavior : scene->_behaviorBatch->behaviors)
			{
				if (behavior->_gameObject == gameObject)
				{
					behavior->_enabled = false;
				}
			}
		}

		scene->DestroyGameObject(gameObject);
	}

	void Behavior::Destroy()
	{
		Destroy(_gameObject);
	}
}