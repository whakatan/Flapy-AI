#include "pch.h"
#include "Assets/Fonts/FontManager.h"
#include "Assets/Fonts/Font.h"

namespace SoulEngine
{
	std::map<std::string, Ref<Font>> FontManager::_fonts;

	Ref<Font> FontManager::Load(const std::string& name, int size)
	{
		std::string directory;
		GetExecutableDirectory(directory);
		directory = directory  + name;

		size_t patSize = name.size();
		size_t findLastDot = name.find_last_of(".");

		auto it = _fonts.find(name);
		if (it != _fonts.end()) return it->second;

		if (std::filesystem::exists(directory))
		{
			std::string extension(name.substr(findLastDot, patSize - findLastDot));

			const std::string fontExt(".ttf");
			if (extension == fontExt)
			{
				Ref<Font> font = CreateRef<Font>(directory, size);
				if (font != nullptr) _fonts.emplace(name, font);

				return font;
			}
		}
		return nullptr;
	}

	Ref<Font> FontManager::Get(const std::string& name, int size)
	{
		auto it = _fonts.find((name + std::to_string(size)));
		if (it != _fonts.end()) return it->second;
		return nullptr;
	}
}