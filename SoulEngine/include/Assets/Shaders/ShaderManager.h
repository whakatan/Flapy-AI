#pragma once
#include "Core.h"
#include <map>
#include <string>

namespace SoulEngine
{
	class Shader;

	class ShaderManager
	{
	public:
		static void Load();
		static Ref<Shader> Get(const std::string& key);

	private:
		static std::string _path;
		static std::map<std::string, Ref<Shader>> _shaders;
		static void Load(const std::string& path);
	};
}