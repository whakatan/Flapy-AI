#pragma once
#include <string.h>

namespace SoulEngine
{
	class Scene;
	struct Entity;

	class SceneSerializer
	{
	public:
		static void Serialize(Scene* scene);
		static Scene* Deserialize(const std::string& name);
	private:
		SceneSerializer() = default;
		SceneSerializer(const SceneSerializer&) = default;
	};
}