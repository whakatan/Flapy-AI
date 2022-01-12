#pragma once
#include "Core.h"

#define ENTT_USE_ATOMIC
#include <entt.hpp>
#include <string>

namespace SoulEngine
{
	struct MainCamera;

	class Behavior;
	class GameObject;
	class Physics2D;
	class BehaviorBatch;

	class Scene
	{
	public:
		Scene(std::string name);
		Scene(const Scene&) = default;
		~Scene();

		GameObject CreateGameObject(const std::string& name);
		GameObject CreateGameObject(const std::string& name, float x, float y, float scaleX, float scaleY);
		void DestroyGameObject(GameObject gameObject);

		void Resize(int width, int height);

		std::string GetName() const { return _name; }
		MainCamera* GetCamera() const { return _camera; }

	private:
		bool _started{ false };
		std::string _name;
		entt::registry _registry;

		MainCamera* _camera;
		Physics2D* _physics2D;
		BehaviorBatch* _behaviorBatch;

		void Start();
		void Update();

		friend class GameObject;
		friend class RendererApi;
		friend class Behavior;
		friend class Physics2D;
		friend class SceneManager;
		friend class SceneSerializer;
		friend class BehaviorBatch;
		friend class ContactListener;
	};
}