#pragma once
#include <vector>

namespace SoulEngine
{
	class Scene;

	class SceneManager
	{

	public:
		static Scene* Create(const std::string& name, bool active = true);

		static Scene* Load(Scene* scene);
		static Scene* Load(const std::string& name, bool active = true);
		static void Destroy(const std::string& name);
		static void Destroy(Scene* scene);
		static void Destroy();

		static constexpr Scene* Active() { return _activeScene; };

	private:
		static Scene* _activeScene;
		static std::vector<Scene*> _scenes;

		SceneManager() = default;
		SceneManager(const SceneManager&) = delete;

		static void Start();
		static void Begin();
		static void Update();
		static void Flush();
		static void Resize(int width, int height);

		friend class Application;
		friend class NativeWindow;
	};
}