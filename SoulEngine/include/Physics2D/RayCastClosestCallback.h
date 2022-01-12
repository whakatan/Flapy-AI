#pragma once
#include "Utils/RaycastHit2D.h"

#include <box2D/b2_world_callbacks.h>

namespace SoulEngine
{
	class RayCastClosestCallback : public b2RayCastCallback
	{
	public:
		int16 ignoreGroup{ 0 };

		RayCastClosestCallback() = default;
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction);

	private:
		RaycastHit2D raycastHit2D;

		friend class Physics2D;
	};
}