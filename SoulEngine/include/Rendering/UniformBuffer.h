#pragma once
#include "Utils/Buffer.h"
#include "Core.h"

namespace SoulEngine
{
	class UniformBuffer
	{
	public:
		static Ref<UniformBuffer> Create(const Buffers& target, int count, int stride);

		UniformBuffer(const Buffers& target, int count, int stride);
        ~UniformBuffer();   

        /// <summary>
        /// Bind buffer.
        /// </summary>
        void Bind() const;

        /// <summary>
        /// Bind buffer.
        /// </summary>
        /// <param name="index"></param>
        void Bind(uint32_t index) const;

        /// <summary>
        /// Updates a subset of a buffer object's data store
        /// </summary>
        /// <param name="data">Specifies a pointer to the new data that will be copied into the data store.</param>
        void SetData(const void* data);

        /// <summary>
        /// Updates a subset of a buffer object's data store
        /// </summary>
        /// <param name="data">Specifies a pointer to the new data that will be copied into the data store.</param>
        /// <param name="count"></param>
        /// <param name="index"></param>
        void SetData(const void* data, size_t count, size_t index);
        
        /// <summary>
        /// Updates a subset of a buffer object's data store
        /// </summary>
        /// <param name="data">Specifies a pointer to the new data that will be copied into the data store.</param>
        /// <param name="count"></param>
        /// <param name="sourceOffset"></param>
        /// <param name="destinationOffset"></param>
        void SetData(const void** data, size_t count = 0, size_t sourceOffset = 0, size_t destinationOffset = 0);

        /// <summary>
        /// Map a buffer object's data store
        /// </summary>
        /// <param name="access">Specifies the access policy, indicating whether it will be possible to read from, write to, or both read from and write to the buffer object's mapped data store.</param>
        void* MapBuffer(const BufferAccess& access);

        /// <summary>
        /// Map a buffer object's data store
        /// </summary>
        void UnmapBuffer();

        /// <summary>
        /// Copy all or part of the data store of a buffer object to the data store of another buffer object.
        /// </summary>
        /// <param name="target"></param>
        /// <param name="sourceOffset"></param>
        /// <param name="destinationOffset"></param>
        /// <param name="size"></param>
        void Copy(const UniformBuffer& target, size_t sourceOffset, size_t destinationOffset, size_t size);

        void SetCount(int count);

		int GetStride() const { return _stride; };
        int GetCount() const { return _count; };

	private:
		unsigned int _id{ 0 };
        int _count{ 0 };
		int _target{ 0 };
		int _stride{ 0 };

        void Create(int count);
        static uint32_t GetOpenGLTarget(const Buffers& target);
	};
}