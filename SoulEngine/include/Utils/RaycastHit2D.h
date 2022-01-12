#pragma once
#include "Scenes/Entities/GameObject.h"

#include <glm/vec2.hpp>

namespace SoulEngine
{
	class GameObject;

	struct RaycastHit2D
	{
		GameObject gameObject;

		bool hit{ false };
		glm::vec2 point{ 0.0f };
		glm::vec2 normal{ 0.0f };
		float fraction{ 1.0f };

		operator bool() { return hit; }

		bool operator ==(const RaycastHit2D& other)
		{
			return hit == other.hit && fraction == other.fraction && point == other.point && normal == other.normal;
		}

		bool operator !=(const RaycastHit2D& other)
		{
			return !(*this == other);
		}
	};
}