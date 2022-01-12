#pragma once
#include "Core.h"
#include <string>

namespace SoulEngine
{
	class Texture
	{
	public:
		static Ref<Texture> Create(const void* data, int width, int height);
		static Ref<Texture> Create(const void* data, int width, int height, int pixelFormat, bool gammaCorrection = false);
		static Ref<Texture> Create(int internalFormat, int format, uint32_t type, int width, int height);

		Texture() = default;
		Texture(const void* data, int width, int height);
		Texture(const void* data, int width, int height, int pixelFormat, bool gammaCorrection = false);
		Texture(int internalFormat, int format, uint32_t type, int width, int height);
		~Texture();

		void Bind(int index = 0) const;
		void SetPixels(int x, int y, int width, int height, const void* pixels, int level = 0);
		void UnBind() const;

		int GetWidth() const { return _width; }
		int GetHeight() const { return _height; }
		std::string GetPath() const { return _path; }

		bool operator ==(const Texture& texture)
		{
			if (this == nullptr) return false;

			return _id == texture._id;
		}

		bool operator !=(const Texture& texture)
		{
			return _id != texture._id;
		}

	private:
		uint32_t _id{ 0 };
		uint32_t _type{ 0 };
		int _width{ 0 };
		int _height{ 0 };
		int _format{ 0 };
		std::string _path{};
	};
}