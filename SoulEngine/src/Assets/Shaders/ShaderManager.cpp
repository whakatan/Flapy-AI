#include "pch.h"
#include "Assets/Shaders/ShaderManager.h"
#include "Assets/Shaders/ShaderIO.h"
#include "Rendering/Shader.h"

namespace SoulEngine
{
	std::string ShaderManager::_path;
	std::map<std::string, Ref<Shader>> ShaderManager::_shaders;

	void ShaderManager::Load()
	{
		std::string path;
		GetExecutableDirectory(path);
		_path = path + "Assets" + PATH_SEPARATOR + "Shaders";
		Load(_path);
	}

	Ref<Shader> ShaderManager::Get(const std::string& key)
	{
		auto find = _shaders.find(key);
		if (find != _shaders.end())
			return find->second;
		else
			throw std::runtime_error("KeyNotFoundException: " + key);
	}

	void ShaderManager::Load(const std::string& path)
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

				const std::string shaderExtension(".vert");
				if (extension == shaderExtension)
				{
					Ref<Shader> shader = ShaderIO::Load(path);
					if (shader != nullptr)
						_shaders.emplace(key, shader);
				}
			}
		}
	}
}