#pragma once
#include "Core.h"

namespace SoulEngine
{
	class Texture;

	class TextureIO
	{
	public:
		static Ref<Texture> Load(const std::string& path, bool flip = false);
	};
}