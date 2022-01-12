#pragma once
#include "Core.h"
#include "Utils/Buffer.h"

namespace SoulEngine
{
	class Shader;

	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(Ref<Shader> shader, const FramebufferProperties& properties);

		Framebuffer(Ref<Shader> shader, const FramebufferProperties& properties);
		Framebuffer(const Framebuffer&) = default;
		~Framebuffer();

		void Bind();
		void Render();
		void ReadPixel(int x, int y, unsigned char* pixel);
		void ReadPixel(int x, int y, int width, int height, float* pixels);

		void SetProperties(const FramebufferProperties& properties);
		const FramebufferProperties& GetProperties() const { return _properties; }

	private:
		uint32_t _id{ 0 };
		uint32_t _intermediateFBO{ 0 };
		uint32_t _vertexArray{ 0 };
		uint32_t _indexBuffer{ 0 };
		uint32_t _colorAttachment{ 0 };
		uint32_t _depthAttachment{ 0 };
		uint32_t _screenTexture{ 0 };

		Ref<Shader> _shader;
		FramebufferProperties _properties;
	};
}