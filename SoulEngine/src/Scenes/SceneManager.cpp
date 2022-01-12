#include "pch.h"
#include "Rendering/RendererApi.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Debug.h"
#include "Scenes/UI.h"
#include "Helpers/SceneSerializer.h"

namespace SoulEngine
{
	Scene* SceneManager::_activeScene{ nullptr };
	std::vector<Scene*> SceneManager::_scenes;

	Scene* SceneManager::Create(const std::string& name, bool active)
	{
		auto& scene = _scenes.emplace_back(new Scene(name));

		if (active) _activeScene = scene;
		return scene;
	}

	void SceneManager::Start()
	{
		CORE_ASSERT(_activeScene != nullptr, "active scene is null");
		RendererApi::Init();

#ifdef _DEBUG
		Debug::Init(128, 128, 256);
#endif
		UI::Init(128, 512, 128, 2048);

		_activeScene->Start();
	}

	Scene* SceneManager::Load(Scene* scene)
	{
		// TODO Load again
		Destroy(scene);
		return nullptr;
	}

	Scene* SceneManager::Load(const std::string& name, bool active)
	{
		auto scene = SceneSerializer::Deserialize(name);
		if (active) _activeScene = scene;
		return scene;
	}

	void SceneManager::Destroy(const std::string& name)
	{
		auto it = std::find_if(_scenes.begin(), _scenes.end(), [&name](const auto& scene) {
			return scene->GetName() == name;
			});
		
		_scenes.erase(it);
		delete it._Ptr;
	}

	void SceneManager::Destroy(Scene* scene)
	{
		auto it = std::find_if(_scenes.begin(), _scenes.end(), [&scene](const auto& s) {
			return scene->GetName() == s->GetName();
			});

		_scenes.erase(it);
		delete it._Ptr;
	}

	void SceneManager::Destroy()
	{
		RendererApi::Destroy();

#ifdef _DEBUG
		Debug::Destroy();
#endif

		for (auto& scene : _scenes) delete scene;
		_scenes.clear();
	}

	void SceneManager::Begin()
	{
#ifdef _DEBUG
		Debug::Begin();
#endif
		UI::Begin();
	}

	void SceneManager::Update()
	{
		CORE_ASSERT(_activeScene != nullptr, "active scene is null");

		_activeScene->Update();
		RendererApi::DrawScene(_activeScene);
	}

	void SceneManager::Flush()
	{
#ifdef _DEBUG
		Debug::Flush();
#endif
		UI::Flush();
	}

	void SceneManager::Resize(int width, int height)
	{
		for (auto& scene : _scenes) scene->Resize(width, height);
	}
}