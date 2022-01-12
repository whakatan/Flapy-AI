#pragma once
#include "Utils/Buffer.h"
#include "Core.h"

namespace SoulEngine
{
	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(int size, const std::vector<LayoutFormat>& layoutFormats);
		static Ref<VertexBuffer> Create(const void* data, int size, const std::vector<LayoutFormat>& layoutFormats);

		VertexBuffer(int size, const std::vector<LayoutFormat>& layoutFormats);
		VertexBuffer(const void* data, int size, const std::vector<LayoutFormat>& layoutFormats);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;

		void SetData(const void* data, uint32_t size);
		
	private:
		uint32_t _id{ 0 };
		static int LayoutTypeToSize(const LayoutFormat& format);
        static int LayoutTypeToOpenGLType(const LayoutFormat& type);
        static int LayoutTypeToCount(const LayoutFormat& type);
        static bool LayoutTypeToNormalize(const LayoutFormat& type);
		static void AddLayout(const LayoutFormat layoutType, int index, int stride, int offset);
	};
}