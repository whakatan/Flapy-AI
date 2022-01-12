#pragma once
#include "Core.h"

namespace SoulEngine
{
	class VertexArray
	{
	public:
		static Ref<VertexArray> Create();

		VertexArray();
		~VertexArray();

		/// <summary>
		/// Bind vertex array.
		/// </summary>
		void Bind() const;

		/// <summary>
		/// Unbind vertex array.
		/// </summary>
		void UnBind() const;

	private:
		unsigned int _id{ 0 };
	};
}