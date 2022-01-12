#include "pch.h"
#include "Rendering/Texture.h"
#include <glad/gl.h>

namespace SoulEngine
{
	Ref<Texture> Texture::Create(const void* data, int width, int height)
	{
		return CreateRef<Texture>(data, width, height);
	}

	Ref<Texture> Texture::Create(const void* data, int width, int height, int pixelFormat, bool gammaCorrection)
	{
		return CreateRef<Texture>(data, width, height, pixelFormat, gammaCorrection);
	}

	Ref<Texture> Texture::Create(int internalFormat, int format, uint32_t type, int width, int height)
	{
		return CreateRef<Texture>(internalFormat, format, type, width, height);
	}

	Texture::Texture(const void* data, int width, int height) : _width(width), _height(height)
	{
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);

		_format = GL_RGBA;
		_type = GL_UNSIGNED_BYTE;
	}

	Texture::Texture(const void* data, int width, int height, int pixelFormat, bool gammaCorrection) : _width(width), _height(height)
	{
		uint32_t format{ 0 };
		uint32_t internalFormat{ 0 };
		if (pixelFormat == 1)
		{
			internalFormat = format = GL_RED;
		}
		else if (pixelFormat == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			format = GL_RGB;
		}
		else if (pixelFormat == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			format = GL_RGBA;
		}

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		_format = format;
		_type = GL_UNSIGNED_BYTE;
	}
	
	Texture::Texture(int internalFormat, int format, uint32_t type, int width, int height) : _width(width), _height(height)
	{
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		_format = format;
		_type = type;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_id);
	}
	
	void Texture::Bind(int index) const
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, _id);
	}
	
	void Texture::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::SetPixels(int x, int y, int width, int height, const void* pixels, int level)
	{
		glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, _format, _type, pixels);
	}
}