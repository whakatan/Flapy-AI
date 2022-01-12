#include "pch.h"
#include "Scenes/Entities/Colliders.h"
#include "Physics2D/Physics2D.h"
#include "Scenes/Entities/Rigidbody2D.h"
#include "Scenes/Entities/Components.h"

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#include <glm/glm.hpp>

namespace SoulEngine
{
	void Collider2D::SetTrigger(bool trigger)
	{
		if (fixture)
			fixture->SetSensor(trigger);
		this->isTrigger = trigger;
	}

	void Collider2D::SetDensity(float density)
	{
		if (fixture)
			fixture->SetDensity(density);
		this->density = density;
	}

	void Collider2D::SetFriction(float friction)
	{
		if (fixture)
			fixture->SetFriction(friction);

		this->friction = friction;
	}

	void Collider2D::SetRestitution(float restitution)
	{
		if (fixture)
			fixture->SetRestitution(restitution);
		this->restitution = restitution;
	}

	void Collider2D::SetRestitutionThreshold(float threshold)
	{
		if (fixture)
			fixture->SetRestitutionThreshold(threshold);
		this->threshold = threshold;
	}

	void BoxCollider2D::SetOffset(const glm::vec2& offset)
	{
		if (fixture)
		{
			auto shape = static_cast<b2PolygonShape*>(fixture->GetShape());
			shape->SetAsBox(transform->scale.x * size.x, transform->scale.y * size.y, { offset.x, offset.y }, 0.0f);
		}
		this->offset = offset;
	}

	void BoxCollider2D::SetSize(const glm::vec2& size)
	{
		if (fixture)
		{
			auto shape = static_cast<b2PolygonShape*>(fixture->GetShape());
			shape->SetAsBox(transform->scale.x * size.x, transform->scale.y * size.y);
		}
		this->size = size;
	}

	void CircleCollider2D::SetOffset(const glm::vec2& offset)
	{
		if (fixture)
		{
			auto shape = static_cast<b2CircleShape*>(fixture->GetShape());
			shape->m_p.Set(offset.x, offset.y);
		}
		this->offset = offset;
	}

	void CircleCollider2D::SetRadius(float radius)
	{
		if (fixture)
		{
			auto shape = static_cast<b2CircleShape*>(fixture->GetShape());
			shape->m_radius = radius * transform->scale.x;
		}

		this->radius = radius;
	}
}