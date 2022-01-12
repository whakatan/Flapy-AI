#include "pch.h"
#include "Rendering/IndexBuffer.h"
#include <glad/gl.h>

namespace SoulEngine
{
	Ref<IndexBuffer> IndexBuffer::Create(const void* data, int size, const IndexType& type)
	{
		return CreateRef<IndexBuffer>(data, size, type);
	}

	IndexBuffer::IndexBuffer(const void* data, int size, const IndexType& type)
	{
		this->type = type;
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	
	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}