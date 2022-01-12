#include "pch.h"
#include "Rendering/UniformBuffer.h"
#include <glad/gl.h>

namespace SoulEngine
{
	Ref<UniformBuffer> UniformBuffer::Create(const Buffers& target, int count, int stride)
	{
		return CreateRef<UniformBuffer>(target, count, stride);
	}

	UniformBuffer::UniformBuffer(const Buffers& target, int count, int stride)
	{
		_stride = stride;
		_target = GetOpenGLTarget(target);
		Create(count);
	}
	
	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &_id);
	}
	
	void UniformBuffer::Bind() const
	{
		glBindBuffer(_target, _id);
	}
	
	void UniformBuffer::Bind(uint32_t index) const
	{
		glBindBufferBase(_target, index, _id);
	}
	
	void UniformBuffer::SetData(const void* data)
	{
		glBufferSubData(_target, 0, _stride, data);
	}
	
	void UniformBuffer::SetData(const void* data, size_t count, size_t index)
	{
		glBufferSubData(_target, index * _stride, count * _stride, data);
	}
	
	void UniformBuffer::SetData(const void** data, size_t count, size_t sourceOffset, size_t destinationOffset)
	{
		glBufferSubData(_target, destinationOffset * _stride, count * _stride, data[sourceOffset]);
	}

	void* UniformBuffer::MapBuffer(const BufferAccess& access)
	{
		return glMapBuffer(_target, static_cast<int>(access));
	}
	
	void UniformBuffer::UnmapBuffer()
	{
		glUnmapBuffer(_target);
	}

	void UniformBuffer::Copy(const UniformBuffer& target, size_t sourceOffset, size_t destinationOffset, size_t size)
	{
		glBindBuffer(GL_COPY_READ_BUFFER, _id);
		glBindBuffer(GL_COPY_WRITE_BUFFER, target._id);
		glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, sourceOffset, destinationOffset, size);
	}

	void UniformBuffer::SetCount(int count)
	{
		glDeleteBuffers(1, &_id);
		Create(count);
	}

	void UniformBuffer::Create(int count)
	{
		_count = count;

		glGenBuffers(1, &_id);
		glBindBuffer(_target, _id);
		glBufferStorage(_target, static_cast<GLsizeiptr>(count) * _stride, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	uint32_t UniformBuffer::GetOpenGLTarget(const Buffers& target)
	{
		switch (target)
		{
		default:
		case Buffers::Vertex:
			return GL_ARRAY_BUFFER;
		case Buffers::Index:
			return GL_ELEMENT_ARRAY_BUFFER;
		case Buffers::Structured:
			return GL_SHADER_STORAGE_BUFFER;
		case Buffers::Constant:
			return GL_UNIFORM_BUFFER;
		}
	}
}