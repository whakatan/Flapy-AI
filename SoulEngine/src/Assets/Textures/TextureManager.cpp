#include "pch.h"
#include "Assets/Textures/TextureManager.h"
#include "Assets/Textures/TextureIO.h"
#include "Rendering/Texture.h"

namespace SoulEngine
{
	std::string TextureManager::_path;
	std::map<std::string, Ref<Texture>> TextureManager::_textures;

	void TextureManager::Load()
	{
		std::string path;
		GetExecutableDirectory(path);
		_path = path + "Assets" + PATH_SEPARATOR + "Shaders";
		Load(_path);
	}

	Ref<Texture> TextureManager::Get(const std::string& key)
	{
		auto find = _textures.find(key);
		if (find != _textures.end())
			return find->second;
		else
			throw std::runtime_error("KeyNotFoundException: " + key);
	}

	Ref<Texture> TextureManager::Create(const std::string& key, const void* data, int width, int height)
	{
		auto find = _textures.find(key);
		if (find != _textures.end())
			return find->second;

		Ref<Texture> texture = Texture::Create(data, width, height);
		if (texture != nullptr)
			_textures.emplace(key, texture);
		return texture;
	}

	Ref<Texture> TextureManager::Load(const std::string& path)
	{
		if (std::filesystem::exists(path))
		{
			if (std::filesystem::is_directory(path))
			{
				for (const auto& entry : std::filesystem::directory_iterator(path))
				{
					Load(entry.path().string());
				}
			}
			else
			{
				size_t size = _path.size();
				size_t find = path.find_last_of(".");
				std::string extension(path.substr(find, path.size()));
				std::string key = path.substr(size + 1, find - size - 1);

				Ref<Texture> texture = TextureIO::Load(path);
				if (texture != nullptr)
					_textures.emplace(key, texture);
				return texture;
			}
		}
		return nullptr;
	}
}