#include "pch.h"
#include "Assets/Textures/TextureIO.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Rendering/Texture.h"

namespace SoulEngine
{
	Ref<Texture> TextureIO::Load(const std::string& path, bool flip)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			Ref<Texture> texture = Texture::Create(data, width, height, nrChannels);
			stbi_image_free(data);
			return texture;
		}

		stbi_image_free(data);
		return nullptr;
	}
}