#include "pch.h"
#include "Physics2D/RayCastClosestCallback.h"
#include "Scenes/SceneManager.h"

#include <box2D/b2_fixture.h>
#include <box2D/b2_body.h>

#include <entt.hpp>

namespace SoulEngine
{
	float RayCastClosestCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
	{
		auto& filter = fixture->GetFilterData();
		if (filter.groupIndex == ignoreGroup)
			return -1;

		if (fraction < raycastHit2D.fraction)
		{
			b2Body* body = fixture->GetBody();

			auto& userData = fixture->GetUserData();
			auto entityIndex = static_cast<entt::entity>(fixture->GetUserData().pointer);

			auto scene = SceneManager::Active();

			raycastHit2D.hit = true;
			raycastHit2D.fraction = fraction;
			raycastHit2D.point = { point.x, point.y };
			raycastHit2D.normal = { normal.x, normal.y };
			raycastHit2D.gameObject = { scene, entityIndex };
		}

		return fraction;
	}
}