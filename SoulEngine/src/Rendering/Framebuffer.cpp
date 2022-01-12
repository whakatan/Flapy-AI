#include "pch.h"
#include "Rendering/Framebuffer.h"
#include "Rendering/Shader.h"

#include <glad/gl.h>

namespace SoulEngine
{
	GLenum GetTextureInternalFormat(const FramebufferTextureFormat& format);
	GLenum GetTextureFormat(const FramebufferTextureFormat& format);
	GLenum GetTextureType(const FramebufferTextureFormat& format);
	GLenum GetDepthInternalFormat(const FramebufferTextureFormat& format);
	GLenum GetDepthAttachment(const FramebufferTextureFormat& format);

	Ref<Framebuffer> Framebuffer::Create(Ref<Shader> shader, const FramebufferProperties& properties)
	{
		return CreateRef<Framebuffer>(shader, properties);
	}

	Framebuffer::Framebuffer(Ref<Shader> shader, const FramebufferProperties& properties) : _shader(shader), _properties(properties)
	{
		shader->Use();
		shader->SetInt("uTexture", 0);

		glGenVertexArrays(1, &_vertexArray);
		glBindVertexArray(_vertexArray);
		
		byte* indices = new byte[6]{ 0, 1, 2, 2, 1, 3 };
		glGenBuffers(1, &_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(byte) * 6, indices, GL_STATIC_DRAW);
		delete[] indices;

		SetProperties(properties);
	}
	
	Framebuffer::~Framebuffer()
	{
		glDeleteTextures(1, &_colorAttachment);
		glDeleteTextures(1, &_depthAttachment);
		glDeleteTextures(1, &_screenTexture);

		glDeleteFramebuffers(1, &_intermediateFBO);
		glDeleteFramebuffers(1, &_id);
		
		glDeleteBuffers(1, &_indexBuffer);
		glDeleteVertexArrays(1, &_vertexArray);
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
	}

	void Framebuffer::Render()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _intermediateFBO);

		glBlitFramebuffer(0, 0, _properties.width, _properties.height, 0, 0, _properties.width, _properties.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		_shader->Use();
		glBindVertexArray(_vertexArray);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _screenTexture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
	}

	void Framebuffer::ReadPixel(int x, int y, unsigned char* pixel)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	}

	void Framebuffer::ReadPixel(int x, int y, int width, int height, float* pixels)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(x, y, width, height, GL_RGB, GL_FLOAT, pixels);
	}

	void Framebuffer::SetProperties(const FramebufferProperties& properties)
	{
		if (properties.height == 0 || properties.width == 0)
			return;

		if (_id != 0)
		{
			glDeleteFramebuffers(1, &_id);
			glDeleteFramebuffers(1, &_intermediateFBO);

			glDeleteTextures(1, &_colorAttachment);
			glDeleteTextures(1, &_screenTexture);
			glDeleteRenderbuffers(1, &_depthAttachment);

			_depthAttachment = 0;
		}
		_properties = properties;

		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);

		GLenum textureTarget = properties.samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

		glGenTextures(1, &_colorAttachment);
		glBindTexture(textureTarget, _colorAttachment);

		if (textureTarget == GL_TEXTURE_2D_MULTISAMPLE)
			glTexImage2DMultisample(textureTarget, properties.samples, GetTextureInternalFormat(properties.textureFormat), properties.width, properties.height, GL_TRUE);
		else
		{
			glTexImage2D(textureTarget, 0, GetTextureInternalFormat(properties.textureFormat), properties.width, properties.height, 0, GetTextureFormat(properties.textureFormat), GetTextureType(properties.textureFormat), nullptr);
			glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_REPEAT);
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, _colorAttachment, 0);
		glBindTexture(textureTarget, 0);

		if (properties.depthFormat != FramebufferTextureFormat::None)
		{
			glGenRenderbuffers(1, &_depthAttachment);
			glBindRenderbuffer(GL_RENDERBUFFER, _depthAttachment);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, properties.samples, GetDepthInternalFormat(properties.depthFormat), properties.width, properties.height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GetDepthAttachment(properties.depthFormat), GL_RENDERBUFFER, _depthAttachment);
		}

		CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenFramebuffers(1, &_intermediateFBO);

		glGenTextures(1, &_screenTexture);
		glBindTexture(GL_TEXTURE_2D, _screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, properties.width, properties.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, _intermediateFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _screenTexture, 0);

		CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLenum GetTextureInternalFormat(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		default:
		case FramebufferTextureFormat::RGB: return GL_RGB;
		case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
		case FramebufferTextureFormat::RGB16F: return GL_RGB16F;
		case FramebufferTextureFormat::RGB32F: return GL_RGB32F;
		case FramebufferTextureFormat::RedInteger: return GL_RED_INTEGER;
		}
	}

	GLenum GetTextureFormat(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		default:
		case FramebufferTextureFormat::RGB: return GL_RGB;
		case FramebufferTextureFormat::RGB16F: return GL_RGB;
		case FramebufferTextureFormat::RGB32F: return GL_RGB;
		case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
		case FramebufferTextureFormat::RedInteger: return GL_R32I;
		}
	}

	GLenum GetTextureType(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		default:
		case FramebufferTextureFormat::RGB16F:
		case FramebufferTextureFormat::RGB32F: return GL_FLOAT;
		case FramebufferTextureFormat::RGB:
		case FramebufferTextureFormat::RGBA8: return GL_UNSIGNED_BYTE;
		case FramebufferTextureFormat::RedInteger: return GL_INT;
		}
	}

	GLenum GetDepthInternalFormat(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		default:
		case FramebufferTextureFormat::Depth: return GL_DEPTH;
		case FramebufferTextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
		case FramebufferTextureFormat::Depth32F: return GL_DEPTH32F_STENCIL8;
		}
	}

	GLenum GetDepthAttachment(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		default:
		case FramebufferTextureFormat::Depth: return GL_DEPTH_ATTACHMENT;
		case FramebufferTextureFormat::Depth24Stencil8:
		case FramebufferTextureFormat::Depth32F: return GL_DEPTH_STENCIL_ATTACHMENT;
		}
	}
}