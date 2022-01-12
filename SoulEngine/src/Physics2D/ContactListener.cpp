#include "pch.h"
#include "Physics2D/ContactListener.h"
#include "Physics2D/Collision.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Entities/BehaviorBatch.h"

#include <box2d/b2_contact.h>

#include <entt.hpp>

namespace SoulEngine
{
	void ContactListener::BeginContact(b2Contact* contact)
	{
		auto fixtureA = contact->GetFixtureA();
		auto fixtureB = contact->GetFixtureB();

		auto scene = SceneManager::Active();

		auto entity1 = static_cast<entt::entity>(fixtureA->GetUserData().pointer);
		auto entity2 = static_cast<entt::entity>(fixtureB->GetUserData().pointer);
		
		GameObject gameObjectA{ scene, entity1 };
		GameObject gameObjectB{ scene, entity2 };

		auto batch = scene->_behaviorBatch;
		if (batch->Contains(gameObjectA))
		{
			if (fixtureB->IsSensor()) batch->OnTriggerEnter(gameObjectA, { contact->IsTouching(), gameObjectB });
			else batch->OnCollisionEnter(gameObjectA, { contact->IsTouching(), gameObjectB });
		}

		if (batch->Contains(gameObjectB))
		{
			if (fixtureA->IsSensor()) batch->OnTriggerEnter(gameObjectB, { contact->IsTouching(), gameObjectA });
			else batch->OnCollisionEnter(gameObjectB, { contact->IsTouching(), gameObjectA });
		}
	}
	
	void ContactListener::EndContact(b2Contact* contact)
	{
		auto fixtureA = contact->GetFixtureA();
		auto fixtureB = contact->GetFixtureB();

		auto scene = SceneManager::Active();

		auto entity1 = static_cast<entt::entity>(fixtureA->GetUserData().pointer);
		auto entity2 = static_cast<entt::entity>(fixtureB->GetUserData().pointer);

		GameObject gameObjectA{ scene, entity1 };
		GameObject gameObjectB{ scene, entity2 };

		auto batch = scene->_behaviorBatch;
		if (batch->Contains(gameObjectA))
		{
			if (fixtureB->IsSensor()) batch->OnTriggerExit(gameObjectA, { contact->IsTouching(), gameObjectB });
			else batch->OnCollisionExit(gameObjectA, { contact->IsTouching(), gameObjectB });
		}

		if (batch->Contains(gameObjectB))
		{
			if (fixtureA->IsSensor()) batch->OnTriggerExit(gameObjectB, { contact->IsTouching(), gameObjectA });
			else batch->OnCollisionExit(gameObjectB, { contact->IsTouching(), gameObjectA });
		}
	}

	void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
	}
	void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}
}