#include "pch.h"
#include "Scenes/Entities/Rigidbody2D.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Entities/Components.h"
#include "Scenes/Entities/Colliders.h"
#include "Scenes/Entities/GameObject.h"
#include "Physics2D/Physics2D.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>

namespace SoulEngine
{
	void Rigidbody2D::SetBodyType(const BodyType& bodyType)
	{
		_bodyType = bodyType;
		if (_physicBody)
			_physicBody->SetType(static_cast<b2BodyType>(bodyType));
	}

	void Rigidbody2D::SetFixedRotation(bool fixedRotation)
	{
		_fixedRotation = fixedRotation;
		if (_physicBody)
			_physicBody->SetFixedRotation(fixedRotation);
	}

	float Rigidbody2D::GetMass() const
	{
		if (!_physicBody) return 0.0f;
		return _physicBody->GetMass();
	}

	float Rigidbody2D::GetInertia() const
	{
		if (!_physicBody) return 0.0f;
		return _physicBody->GetInertia();
	}

	void Rigidbody2D::SetMass(float mass, const glm::vec2& center, float I)
	{
		b2MassData massData{ mass, { center.x, center.y }, I };
		_physicBody->SetMassData(&massData);
	}

	float Rigidbody2D::GetGravityScale() const
	{
		if (!_physicBody) return _gravityScale;
		return _physicBody->GetGravityScale();
	}

	void Rigidbody2D::SetGravityScale(float scale)
	{
		if (_physicBody)
			_physicBody->SetGravityScale(scale);
		_gravityScale = scale;
	}

	float Rigidbody2D::GetDamping() const
	{
		if (!_physicBody) return 0.0f;
		return _physicBody->GetLinearDamping();
	}

	void Rigidbody2D::SetDamping(float damping)
	{
		_physicBody->SetLinearDamping(damping);
	}

	float Rigidbody2D::GetAngularDamping() const
	{
		if (!_physicBody) return 0.0f;
		return _physicBody->GetAngularDamping();
	}

	void Rigidbody2D::SetAngularDamping(float damping)
	{
		_physicBody->SetAngularDamping(damping);
	}

	glm::vec2 Rigidbody2D::GetPosition()
	{
		auto& position = _physicBody->GetPosition();
		return { position.x, position.y };
	}

	glm::vec2 Rigidbody2D::GetTransform()
	{
		auto& transform = _physicBody->GetTransform();
		return { transform.p.x, transform.p.y };
	}

	void Rigidbody2D::SetTransform(const glm::vec2& position, float angle)
	{
		if (_physicBody)
			_physicBody->SetTransform({ position.x, position.y }, angle);
		else
		{
			transform->position.x = position.x;
			transform->position.y = position.y;
			transform->rotation.z = angle;
		}
	}

	bool Rigidbody2D::IsBullet() const
	{
		if (!_physicBody) return _bullet;

		return _physicBody->IsBullet();
	}

	void Rigidbody2D::SetBullet(bool bullet)
	{
		if (_physicBody)
			_physicBody->SetBullet(bullet);
		_bullet = bullet;
	}

	glm::vec2 Rigidbody2D::GetVelocity() const
	{
		if (!_physicBody) return { 0.0f, 0.0f };
		const auto& velocity = _physicBody->GetLinearVelocity();
		return { velocity.x, velocity.y };
	}

	float Rigidbody2D::GetAngularVelocity() const
	{
		if (!_physicBody) return 0.0f;
		return _physicBody->GetAngularVelocity();
	}

	void Rigidbody2D::SetVelocity(float x, float y)
	{
		if (_physicBody)
			_physicBody->SetLinearVelocity({ x, y });
	}

	void Rigidbody2D::SetVelocity(const glm::vec2& velocity)
	{
		if (_physicBody)
			_physicBody->SetLinearVelocity({ velocity.x, velocity.y });
	}

	glm::vec2 Rigidbody2D::GetWorldCenter() const
	{
		const auto& worldPoint = _physicBody->GetWorldCenter();
		return { worldPoint.x, worldPoint.y };
	}

	glm::vec2 Rigidbody2D::GetWorldPoint(const glm::vec2& localPoint)
	{
		auto worldPoint = _physicBody->GetWorldPoint({ localPoint.x, localPoint.y });
		return { worldPoint.x, worldPoint.y };
	}

	void Rigidbody2D::SetAngularVelocity(float velocity)
	{
		_physicBody->SetAngularVelocity(velocity);
	}

	void Rigidbody2D::Create()
	{
		if (!_physicBody) return;
		
		Physics2D::_last->_world->DestroyBody(_physicBody);
		GameObject gameObject = { SceneManager::Active(), static_cast<entt::entity>(_id) };
		Create(Physics2D::_last->_world, gameObject);
	}

	bool Rigidbody2D::IsAwake() const
	{
		if (!_physicBody) return _isAwake;
		return _physicBody->IsAwake();
	}

	void Rigidbody2D::SetAwake(bool awake)
	{
		if (_physicBody)
			_physicBody->SetAwake(awake);
		_isAwake = awake;
	}

	bool Rigidbody2D::IsEnabled() const
	{
		if (!_physicBody) return _enabled;

		return _physicBody->IsEnabled();
	}

	void Rigidbody2D::SetEnabled(bool enabled)
	{
		if (_physicBody)
			_physicBody->SetEnabled(enabled);
		_enabled = enabled;
	}

	void Rigidbody2D::AddForce(const glm::vec2& force, const ForceMode2D& forceMode)
	{
		if (forceMode == ForceMode2D::Impulse)
			_physicBody->ApplyLinearImpulseToCenter({ force.x, force.y }, true);
		else
			_physicBody->ApplyForceToCenter({ force.x, force.y }, true);
	}

	void Rigidbody2D::AddForce(const glm::vec2& force, const glm::vec2& point, const ForceMode2D& forceMode)
	{
		if (forceMode == ForceMode2D::Impulse)
			_physicBody->ApplyLinearImpulse({ force.x, force.y }, { point.x, point.y }, true);
		else
			_physicBody->ApplyForce({ force.x, force.y }, { point.x, point.y }, true);
	}
	
	void Rigidbody2D::Create(b2World* world, GameObject& gameObject)
	{
		auto& transform = gameObject.GetTransform();

		_id = static_cast<uint32_t>(gameObject);
		b2BodyDef bodyDef;
		bodyDef.type = static_cast<b2BodyType>(_bodyType);
		bodyDef.position.Set(transform.position.x, transform.position.y);
		bodyDef.angle = glm::radians(transform.rotation.z);
		bodyDef.userData.pointer = _id;
		bodyDef.enabled = _enabled;
		bodyDef.awake = _isAwake;
		bodyDef.bullet = _bullet;
		bodyDef.fixedRotation = _fixedRotation;
		bodyDef.gravityScale = _gravityScale;

		if (gameObject.HasComponent<BoxCollider2D>())
		{
			auto& boxCollider2D = gameObject.GetComponent<BoxCollider2D>();
			boxCollider2D.transform = &transform;

			auto offset = boxCollider2D.GetOffset();
			auto size = boxCollider2D.GetSize();

			b2PolygonShape boxShape;
			boxShape.SetAsBox(transform.scale.x * size.x, transform.scale.y * size.y, { offset.x, offset.y }, 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.isSensor = boxCollider2D.IsTrigger();
			fixtureDef.density = boxCollider2D.GetDensity();
			fixtureDef.friction = boxCollider2D.GetFriction();
			fixtureDef.restitution = boxCollider2D.GetRestitution();
			fixtureDef.restitutionThreshold = boxCollider2D.GetRestitutionThreshold();
			fixtureDef.userData.pointer = _id;
			fixtureDef.filter.groupIndex = groupIndex;

			b2Body* body = world->CreateBody(&bodyDef);

			_physicBody = body;
			boxCollider2D.fixture = body->CreateFixture(&fixtureDef);
		}
		else if (gameObject.HasComponent<CircleCollider2D>())
		{
			auto& circleCollider = gameObject.GetComponent<CircleCollider2D>();
			circleCollider.transform = &transform;

			bodyDef.position.Set(transform.position.x, transform.position.y);

			b2CircleShape circleShape;
			circleShape.m_radius = circleCollider.GetRadius() * transform.scale.x;
			circleShape.m_p.Set(circleCollider.offset.x, circleCollider.offset.y);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = circleCollider.GetDensity();
			fixtureDef.friction = circleCollider.GetFriction();
			fixtureDef.restitution = circleCollider.GetRestitution();
			fixtureDef.restitutionThreshold = circleCollider.GetRestitutionThreshold();
			fixtureDef.userData.pointer = _id;
			fixtureDef.filter.groupIndex = groupIndex;

			b2Body* body = world->CreateBody(&bodyDef);

			_physicBody = body;
			circleCollider.fixture = body->CreateFixture(&fixtureDef);
		}
		else
		{
			bodyDef.position.Set(transform.position.x, transform.position.y);
			b2Body* body = world->CreateBody(&bodyDef);
			_physicBody = body;
		}
	}
}