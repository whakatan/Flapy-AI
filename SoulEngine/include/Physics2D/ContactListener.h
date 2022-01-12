#pragma once

#include <box2d/b2_world_callbacks.h>

namespace SoulEngine
{
	class ContactListener : public b2ContactListener
	{
	public:
		ContactListener() = default;

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	};
}