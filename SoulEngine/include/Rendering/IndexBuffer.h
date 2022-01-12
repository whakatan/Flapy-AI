#pragma once
#include "Utils/Buffer.h"
#include "Core.h"

namespace SoulEngine
{
	class IndexBuffer
	{
	public:
		static Ref<IndexBuffer> Create(const void* data, int size, const IndexType& type);
		
		IndexBuffer(const void* data, int size, const IndexType& type);
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

		IndexType GetType() const { return type; }
		void SetType(const IndexType type) { this->type = type; }
		
	private:
		IndexType type;
		uint32_t _id{ 0 };
	};
}