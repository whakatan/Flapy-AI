#include "pch.h"
#include "Assets/Shaders/ShaderIO.h"
#include "Rendering/Shader.h"

namespace SoulEngine
{
	Ref<Shader> ShaderIO::Load(const std::string& path)
	{
		size_t find = path.find_last_of(".");
		std::string extension = path.substr(find, path.size());
		std::string vertPath(path);
		std::string fragPath = vertPath.substr(0, find) + ".frag";

		if (!std::filesystem::exists(fragPath))
			return nullptr;

		std::string vertexSource;
		std::string fragmentSource;

		std::ifstream vertexFile(vertPath, std::ios::in | std::ios::binary | std::ios::ate);
		if (vertexFile.is_open())
		{
			std::streampos size = vertexFile.tellg();
			vertexSource.resize(size);

			vertexFile.seekg(0, std::ios::beg);
			vertexFile.read(&vertexSource[0], size);
			vertexFile.close();

			std::ifstream fragmentFile(fragPath, std::ios::in | std::ios::binary | std::ios::ate);
			if (fragmentFile.is_open())
			{
				size = fragmentFile.tellg();
				fragmentSource.resize(size);

				fragmentFile.seekg(0, std::ios::beg);
				fragmentFile.read(&fragmentSource[0], size);
				fragmentFile.close();

				auto seperator = path.find_last_of(PATH_SEPARATOR);
				std::string debugName = fragPath.substr(seperator + 1, fragPath.size() - seperator - 1 - extension.size());
				return Shader::Create(debugName, vertexSource, fragmentSource);
			}
			return nullptr;
		}
		return nullptr;
	}
}