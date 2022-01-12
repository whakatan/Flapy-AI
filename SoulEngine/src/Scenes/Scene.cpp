#include "pch.h"
#include "Scenes/Scene.h"
#include "Scenes/MainCamera.h"
#include "Scenes/Entities/GameObject.h"
#include "Scenes/Entities/Components.h"
#include "Scenes/Entities/Rigidbody2D.h"
#include "Physics2D/Physics2D.h"
#include "Window/Time.h"

namespace SoulEngine
{
    std::vector<GameObject> createdGameObject;
    std::vector<GameObject> destroyedGameObject;

    Scene::Scene(std::string name) : _name(std::move(name))
    {
        _physics2D = new Physics2D();
        _behaviorBatch = new BehaviorBatch(this);
        _camera = new MainCamera(this, _registry.create());
    }

    Scene::~Scene()
    {
        delete _physics2D;
        delete _behaviorBatch;
        delete _camera;
        _registry.clear();
    }

    GameObject Scene::CreateGameObject(const std::string& name)
    {
        GameObject gameObject(this, _registry.create());
        gameObject.AddComponent<Id>();
        gameObject.AddComponent<Tag>(name.empty() ? "GameObject" : name);
        gameObject.AddComponent<Transform>();

        if (_started)
            createdGameObject.push_back(gameObject);
        return gameObject;
    }

    GameObject Scene::CreateGameObject(const std::string& name, float x, float y, float scaleX, float scaleY)
    {
        GameObject gameObject(this, _registry.create());
        gameObject.AddComponent<Id>();
        gameObject.AddComponent<Tag>(name.empty() ? "GameObject" : name);
        gameObject.AddComponent<Transform>(x, y, scaleX, scaleY);

        if (_started)
            createdGameObject.push_back(gameObject);

        return gameObject;
    }

    void Scene::DestroyGameObject(GameObject gameObject)
    {
        destroyedGameObject.emplace_back(gameObject);
    }

    void Scene::Resize(int width, int height)
    {
        _camera->SetAspect(static_cast<float>(width) / static_cast<float>(height));
    }

    void Scene::Start()
    {
        _behaviorBatch->Start();
        _physics2D->Start(this);
        _started = true;
    }

    void Scene::Update()
    {
        for (auto& gameObject : destroyedGameObject)
        {
            _behaviorBatch->Destroy(gameObject);
            _physics2D->DestroyBody(gameObject);
            _registry.destroy(gameObject);
        }
        destroyedGameObject.clear();

        for (auto& gameObject : createdGameObject)
        {
            _behaviorBatch->Start(gameObject);
            _physics2D->CreateBody(gameObject);
        }
        createdGameObject.clear();

        _behaviorBatch->Update();
        _physics2D->FixedUpdate(this);
        _behaviorBatch->LateUpdate();
    }
}