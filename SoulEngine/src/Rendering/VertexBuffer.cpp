#include "pch.h"
#include "Rendering/VertexBuffer.h"
#include <glad/gl.h>

namespace SoulEngine
{
    Ref<VertexBuffer> VertexBuffer::Create(int size, const std::vector<LayoutFormat>& layoutFormats)
    {
        return CreateRef<VertexBuffer>(size, layoutFormats);
    }

    Ref<VertexBuffer> VertexBuffer::Create(const void* data, int size, const std::vector<LayoutFormat>& layoutFormats)
    {
        return CreateRef<VertexBuffer>(data, size, layoutFormats);
    }
    
    VertexBuffer::VertexBuffer(int size, const std::vector<LayoutFormat>& layoutFormats)
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

        int stride = 0;
        for (auto& format : layoutFormats)
            stride += LayoutTypeToSize(format);

        int index = 0;
        int offset = 0;
        for (auto& format : layoutFormats)
        {
            AddLayout(format, index++, stride, offset);
            offset += LayoutTypeToSize(format);
        }
    }

    VertexBuffer::VertexBuffer(const void* data, int size, const std::vector<LayoutFormat>& layoutFormats)
	{
		glGenBuffers(1, &_id);
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        int stride = 0;
        for (auto& format : layoutFormats)
            stride += LayoutTypeToSize(format);

        int index = 0;
        int offset = 0;
        for (auto& format : layoutFormats)
        {
            AddLayout(format, index++, stride, offset);
            offset += LayoutTypeToSize(format);
        }
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::AddLayout(const LayoutFormat layoutType, int index, int stride, int offset)
	{
        if (layoutType == LayoutFormat::Int || layoutType == LayoutFormat::Int2 ||
            layoutType == LayoutFormat::Int3 || layoutType == LayoutFormat::Int4)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribIPointer(index,
                VertexBuffer::LayoutTypeToCount(layoutType),
                VertexBuffer::LayoutTypeToOpenGLType(layoutType),
                stride,
                reinterpret_cast<const void*>(static_cast<intptr_t>(offset)));
        }
        else if (layoutType == LayoutFormat::Mat3 || layoutType == LayoutFormat::Mat4)
        {
            int count = VertexBuffer::LayoutTypeToCount(layoutType);
            for (int j = 0; j < count; ++j)
            {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, count,
                    VertexBuffer::LayoutTypeToOpenGLType(layoutType),
                    VertexBuffer::LayoutTypeToNormalize(layoutType),
                    stride,
                    reinterpret_cast<const void*>(static_cast<intptr_t>(offset * sizeof(float) * count * j)));
                glVertexAttribDivisor(index, 1);
            }
        }
        else
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                VertexBuffer::LayoutTypeToCount(layoutType),
                VertexBuffer::LayoutTypeToOpenGLType(layoutType),
                VertexBuffer::LayoutTypeToNormalize(layoutType),
                stride,
                reinterpret_cast<const void*>(static_cast<intptr_t>(offset)));
        }
	}

    int VertexBuffer::LayoutTypeToSize(const LayoutFormat& format)
    {
        switch (format)
        {
        case LayoutFormat::Float: return sizeof(float);
        case LayoutFormat::Float2: return sizeof(float) * 2;
        case LayoutFormat::Float3: return sizeof(float) * 3;
        case LayoutFormat::Float4: return sizeof(float) * 4;

        case LayoutFormat::Int: return sizeof(int);
        case LayoutFormat::Int2: return sizeof(int) * 2;
        case LayoutFormat::Int3: return sizeof(int) * 3;
        case LayoutFormat::Int4: return sizeof(int) * 4;

        case LayoutFormat::UInt: return sizeof(uint32_t);
        case LayoutFormat::UInt2: return sizeof(uint32_t) * 2;
        case LayoutFormat::UInt3: return sizeof(uint32_t) * 3;
        case LayoutFormat::UInt4: return sizeof(uint32_t) * 4;

        case LayoutFormat::Bool: return sizeof(bool);
        case LayoutFormat::Bool2: return sizeof(bool) * 2;
        case LayoutFormat::Bool3: return sizeof(bool) * 3;
        case LayoutFormat::Bool4: return sizeof(bool) * 4;
        case LayoutFormat::Mat3: return sizeof(float) * 9;
        case LayoutFormat::Mat4: return sizeof(float) * 16;
        default: return 0;
        }
    }

    int VertexBuffer::LayoutTypeToOpenGLType(const LayoutFormat& type)
    {
        switch (type)
        {
        case LayoutFormat::Float:
        case LayoutFormat::Float2:
        case LayoutFormat::Float3:
        case LayoutFormat::Float4:
        case LayoutFormat::Mat3:
        case LayoutFormat::Mat4:
            return GL_FLOAT;
        case LayoutFormat::UInt:
        case LayoutFormat::UInt2:
        case LayoutFormat::UInt3:
        case LayoutFormat::UInt4:
            return GL_UNSIGNED_INT;
        case LayoutFormat::Int:
        case LayoutFormat::Int2:
        case LayoutFormat::Int3:
        case LayoutFormat::Int4:
        case LayoutFormat::Bool:
        case LayoutFormat::Bool2:
        case LayoutFormat::Bool3:
        case LayoutFormat::Bool4:
            return GL_INT;
        default:
            return 0;
        }
    }

    int VertexBuffer::LayoutTypeToCount(const LayoutFormat& type)
    {
        switch (type)
        {
        case LayoutFormat::Float:
        case LayoutFormat::Int:
        case LayoutFormat::UInt:
        case LayoutFormat::Bool: return 1;

        case LayoutFormat::Float2:
        case LayoutFormat::Int2:
        case LayoutFormat::UInt2:
        case LayoutFormat::Bool2: return 2;

        case LayoutFormat::Float3:
        case LayoutFormat::Int3:
        case LayoutFormat::UInt3:
        case LayoutFormat::Bool3:
        case LayoutFormat::Mat3: return 3;

        case LayoutFormat::Float4:
        case LayoutFormat::Int4:
        case LayoutFormat::UInt4:
        case LayoutFormat::Bool4:
        case LayoutFormat::Mat4: return 4;
        default: return 0;
        }
    }

    bool VertexBuffer::LayoutTypeToNormalize(const LayoutFormat& type)
    {
        switch (type)
        {
        case LayoutFormat::Bool:
        case LayoutFormat::Bool2:
        case LayoutFormat::Bool3:
        case LayoutFormat::Bool4:
            return true;
        default: return false;
        }
    }
}