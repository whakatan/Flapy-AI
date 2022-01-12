#pragma once
#include <glm/vec2.hpp>

class b2Fixture;

namespace SoulEngine
{
	struct Transform;
	struct Rigidbody2D;

	struct Collider2D
	{
	public:
		glm::vec2 GetOffset() const { return offset; }
		virtual void SetOffset(const glm::vec2& offset) = 0;

		bool IsTrigger() const { return isTrigger; }
		void SetTrigger(bool trigger);

		float GetDensity() const { return density; }
		void SetDensity(float density);

		float GetFriction() const { return friction; }
		void SetFriction(float friction);

		float GetRestitution() const { return restitution; }
		void SetRestitution(float restitution);

		float GetRestitutionThreshold() const { return threshold; }
		void SetRestitutionThreshold(float threshold);

	protected:
		bool isTrigger{ false };
		float density{ 1.0f };
		float friction{ 0.5f };
		float restitution{ 0.0f };
		float threshold{ 0.5f };
		glm::vec2 offset{ 0.0f, 0.0f };
		b2Fixture* fixture{ nullptr };
		Transform* transform{ nullptr };

		friend class GameObject;
	};

	struct BoxCollider2D : public Collider2D
	{
	public:
		void SetOffset(const glm::vec2& offset) override;

		glm::vec2 GetSize() const { return size; }
		void SetSize(const glm::vec2& size);

	protected:
		glm::vec2 size{ 0.5f, 0.5f };

		friend struct Rigidbody2D;
	};

	struct CircleCollider2D : public Collider2D
	{
	public:
		void SetOffset(const glm::vec2& offset) override;

		float GetRadius() const { return radius; }
		void SetRadius(float radius);

	protected:
		float radius{ 0.5f };

		friend struct Rigidbody2D;
	};
}