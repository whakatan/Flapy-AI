#pragma once
#include "Core.h"
#include <string>

namespace SoulEngine
{
	class Shader;

	class ShaderIO
	{
	public:
		static Ref<Shader> Load(const std::string& path);
	};
}