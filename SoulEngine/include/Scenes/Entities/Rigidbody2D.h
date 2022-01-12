#pragma once
#include "Utils/Physics2D.h"

#include <glm/vec2.hpp>

class b2Body;
class b2World;

namespace SoulEngine
{
	class GameObject;
	struct Transform;

	struct Rigidbody2D
	{
	public:
		signed short groupIndex = 0;

		BodyType GetBodyType() const { return _bodyType; }
		void SetBodyType(const BodyType& bodyType);

		bool IsFixedRotation() const { return _fixedRotation; }
		void SetFixedRotation(bool fixedRotation);

		float GetInertia() const;
		float GetMass() const;
		void SetMass(float mass, const glm::vec2& center, float I);

		float GetGravityScale() const;
		void SetGravityScale(float scale);

		float GetDamping() const;
		void SetDamping(float damping);

		float GetAngularDamping() const;
		void SetAngularDamping(float damping);

		glm::vec2 GetPosition();

		glm::vec2 GetTransform();
		void SetTransform(const glm::vec2& position, float angle);
		
		bool IsBullet() const;
		void SetBullet(bool bullet);

		glm::vec2 GetVelocity() const;
		void SetVelocity(float x, float y);
		void SetVelocity(const glm::vec2& velocity);

		glm::vec2 GetWorldCenter() const;
		glm::vec2 GetWorldPoint(const glm::vec2& localPoint);

		float GetAngularVelocity() const;
		void SetAngularVelocity(float velocity);

		void Create();

		bool IsAwake() const;
		void SetAwake(bool awake);

		bool IsEnabled() const;
		void SetEnabled(bool enabled);

		void AddForce(const glm::vec2& force, const ForceMode2D& forceMode);
		void AddForce(const glm::vec2& force, const glm::vec2& point, const ForceMode2D& forceMode);

	private:
		uint32_t _id{ 0 };

		bool _enabled{ true };
		bool _isAwake{ true };
		bool _fixedRotation{ false };
		bool _bullet{ false };
		float _gravityScale{ 1.0f };

		b2Body* _physicBody{ nullptr };
		Transform* transform{ nullptr };
		BodyType _bodyType{ BodyType::Static };

		void Create(b2World* world, GameObject& gameObject);

		friend class Scene;
		friend class Physics2D;
		friend class GameObject;
	};
}