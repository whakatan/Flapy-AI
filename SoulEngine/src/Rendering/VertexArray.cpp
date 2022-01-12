#include "pch.h"
#include "Rendering/VertexArray.h"
#include <glad/gl.h>

namespace SoulEngine
{
	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<VertexArray>();
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &_id);
		glBindVertexArray(_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &_id);
	}
	
	void VertexArray::Bind() const
	{
		glBindVertexArray(_id);
	}
	
	void VertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
}