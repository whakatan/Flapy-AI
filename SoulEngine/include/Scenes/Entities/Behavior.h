#pragma once
#include "Scenes/Entities/GameObject.h"

namespace SoulEngine
{
	struct Collision2D;

	class Behavior
	{
	public:
		static GameObject FindGameObject(const std::string& tag);
		static void Destroy(GameObject gameObject);

		Behavior() = default;
		Behavior(const Behavior&) = default;
		virtual ~Behavior() = default;

		GameObject GetGameObject() const { return _gameObject; }

		Uuid GetId() { return _gameObject.GetId(); }
		std::string GetTag() { return _gameObject.GetTag(); }
		Transform& GetTransform() { return _gameObject.GetTransform(); }

		template <typename T>
		bool HasComponent()
		{
			return _gameObject.HasComponent<T>();
		}

		template <typename T>
		T& AddComponent()
		{
			return _gameObject.AddComponent<T>();
		}

		template <typename T>
		T& GetComponent()
		{
			return _gameObject.GetComponent<T>();
		}

		template <typename T>
		T& RemoveComponent()
		{
			return _gameObject.RemoveComponent<T>();
		}

	protected:
		bool GetEnable() const { return _enabled; }
		void SetEnable(bool enable) { _enabled = enable; }
		void Destroy();

		virtual void Create() { }
		virtual void Start() { }
		virtual void Update() { }
		virtual void FixedUpdate() { }
		virtual void LateUpdate() { }
		virtual void OnDestroy() { }

		virtual void OnCollisionEnter(const Collision2D& collision) { }
		virtual void OnCollisionExit(const Collision2D& collision) { }

		virtual void OnTriggerEnter(const Collision2D& collision) { }
		virtual void OnTriggerExit(const Collision2D& collision) { }

	private:
		bool _enabled{ false };
		GameObject _gameObject;

		friend class BehaviorBatch;
	};
}