#pragma once
#include "Core.h"
#include <map>
#include <string>

namespace SoulEngine
{
	class Font;

	class FontManager
	{
	public:
		static Ref<Font> Load(const std::string& name, int size);
		static Ref<Font> Get(const std::string& name, int size);

	private:
		static std::map<std::string, Ref<Font>> _fonts;
	};
}