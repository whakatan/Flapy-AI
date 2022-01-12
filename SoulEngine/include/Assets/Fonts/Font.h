#pragma once
#include "Core.h"
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <map>

namespace SoulEngine
{
	class Texture;

	struct Character
	{
		glm::ivec2 Advance;
		glm::ivec2 Bearing;
		glm::ivec2 Size;
		glm::vec4 TextureRect;
	};

	class Font
	{
	public:
		Font(const std::string& path, int size);

		int GetSize() const { return _size; }
		std::string GetName() const { return _name; }
		std::string GetPath() const { return _path; }

		bool operator ==(const Font& other) const
		{
			return _size == other._size && _name == other._name;
		}

		bool operator !=(const Font& other) const
		{
			return (*this == other);
		}

	private:
		int _size;
		std::string _name;
		std::string _path;

		Ref<Texture> _texture;
		mutable std::map<char, Character*> characters;

#ifdef _DEBUG
		friend class Debug;
#endif // _DEBUG

		friend class UI;
		friend class Renderer2D;
	};
}