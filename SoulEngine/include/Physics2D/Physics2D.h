#pragma once
#include "Utils/RaycastHit2D.h"

#include <glm/vec2.hpp>

class b2World;

namespace SoulEngine
{
	class Scene;
	class GameObject;
	class ContactListener;

	struct Entity;
	struct Rigidbody2D;

	class Physics2D
	{
	public:
		Physics2D() = default;
		~Physics2D();
		
		static RaycastHit2D RayCast(const glm::vec2& point1, const glm::vec2& point2, int ignoreGroup = -1);
		static RaycastHit2D RayCast(const glm::vec2& point, const glm::vec2& direction, float maxDistance, int ignoreGroup = -1);

	private:
		static Physics2D* _last;

		b2World* _world{ nullptr };
		int _velocityIterations{ 8 };
		int _positionIterations{ 3 };
		glm::vec2 _gravity{ 0.0f, -9.8f };
		ContactListener* _contactListener{ nullptr };

		Physics2D(const Physics2D&) = delete;
		void Start(Scene* scene);
		void FixedUpdate(Scene* scene);

		void CreateBody(GameObject& gameObject);
		void DestroyBody(GameObject& gameObject);

		friend class Scene;
		friend struct Rigidbody2D;
	};
}