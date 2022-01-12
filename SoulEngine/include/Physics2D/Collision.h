#pragma once
#include "Scenes/Entities/GameObject.h"

namespace SoulEngine
{
	struct Collision2D
	{
	public:

		bool IsTouching() const { return _isTouching; };
		GameObject GetGameObject() const { return _gameObject; };

	private:
		bool _isTouching;
		GameObject _gameObject;

		Collision2D(bool isTouching, GameObject gameObject) : _isTouching(isTouching), _gameObject(gameObject) { }

		friend class ContactListener;
	};
}