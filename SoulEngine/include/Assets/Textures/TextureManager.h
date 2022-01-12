#pragma once
#include "Core.h"
#include <map>
#include <string>

namespace SoulEngine
{
	class Texture;

	class TextureManager
	{
	public:
		static void Load();
		static Ref<Texture> Load(const std::string& path);
		static Ref<Texture> Get(const std::string& key);
		static Ref<Texture> Create(const std::string& key, const void* data, int width, int height);

	private:
		static std::string _path;
		static std::map<std::string, Ref<Texture>> _textures;
	};
}