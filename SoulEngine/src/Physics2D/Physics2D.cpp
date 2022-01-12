#include <pch.h>
#include "Physics2D/Physics2D.h"
#include "Physics2D/ContactListener.h"
#include "Physics2D/RayCastClosestCallback.h"
#include "Physics2D/QueryCallback.h"
#include "Scenes/Scene.h"
#include "Scenes/Debug.h"
#include "Scenes/Entities/GameObject.h"
#include "Scenes/Entities/Components.h"
#include "Scenes/Entities/Rigidbody2D.h"
#include "Scenes/Entities/Colliders.h"
#include "Scenes/Entities/BehaviorBatch.h"
#include "Window/Time.h"

#include <entt.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

namespace SoulEngine
{
    //std::thread _fixedUpdate;
    //std::atomic<bool> _executeFixed{ true };
    Physics2D* Physics2D::_last{ nullptr };

    Physics2D::~Physics2D()
    {
        //_executeFixed = false;
        //if (_fixedUpdate.joinable())
        //    _fixedUpdate.join();
        delete _contactListener;
        delete _world;
    }

    RaycastHit2D Physics2D::RayCast(const glm::vec2& point, const glm::vec2& direction, float maxDistance, int ignoreGroup)
    {
        RayCastClosestCallback callback;
        callback.ignoreGroup = ignoreGroup;
        _last->_world->RayCast(&callback, { point.x, point.y }, { point.x + (direction.x * maxDistance), point.y + (direction.y * maxDistance) });
        return callback.raycastHit2D;
    }

    RaycastHit2D Physics2D::RayCast(const glm::vec2& point1, const glm::vec2& point2, int ignoreGroup)
    {
        RayCastClosestCallback callback;
        callback.ignoreGroup = ignoreGroup;
        _last->_world->RayCast(&callback, { point1.x, point1.y }, { point2.x, point2.y });
        return callback.raycastHit2D;
    }

	void Physics2D::Start(Scene* scene)
	{
        _last = this;

		_world = new b2World({ _gravity.x, _gravity.y });
        _contactListener = new ContactListener();
        _world->SetContactListener(_contactListener);
        
        auto view = scene->_registry.view<Rigidbody2D>();
        for (auto e : view)
        {
            GameObject gameObject{ scene, e };

            auto& rigidbody2D = view.get<Rigidbody2D>(e);
            rigidbody2D.Create(_world, gameObject);
        }

        //_fixedUpdate = std::thread(&Physics2D::FixedUpdate, this, scene);
        //_fixedUpdate.detach();
	}

    void Physics2D::FixedUpdate(Scene* scene)
    {
        //using clock_t = std::chrono::high_resolution_clock;
        //clock_t::time_point first;
        //
        //float dt = 0.0f;
        //const int wait = 20;
        //while (_executeFixed)
        //{
        //    Time::SetFixedDeltaTime();
        //
        scene->_behaviorBatch->FixedUpdate();

        _world->Step(Time::GetDeltaTime(), _velocityIterations, _positionIterations);

        auto group = scene->_registry.group<Rigidbody2D>(entt::get<Transform>);
        for (auto entity : group)
        {
            auto [rigidbody2D, transform] = group.get<Rigidbody2D, Transform>(entity);
            b2Body* body = rigidbody2D._physicBody;

            if (body)
            {
                const auto& position = body->GetPosition();
                transform.position.x = position.x;
                transform.position.y = position.y;
                transform.rotation.z = glm::degrees(body->GetAngle());
            }
        }
            
        //    dt += Time::GetFixedDeltaTime();
        //    if (dt < wait)
        //    {
        //        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(wait - dt)));
        //        dt = 0.0f;
        //    }
        //}
    }

    void Physics2D::CreateBody(GameObject& gameObject)
    {
        if (gameObject.HasComponent<Rigidbody2D>())
        {
            auto& rigidbody2D = gameObject.GetComponent<Rigidbody2D>();
            rigidbody2D.Create(_world, gameObject);
        }
    }

    void Physics2D::DestroyBody(GameObject& gameObject)
    {
        if (gameObject.HasComponent<Rigidbody2D>())
        {
            auto& rigidbody2D = gameObject.GetComponent<Rigidbody2D>();
            _world->DestroyBody(rigidbody2D._physicBody);
        }
    }
}