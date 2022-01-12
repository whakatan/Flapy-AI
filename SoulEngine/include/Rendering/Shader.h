#pragma once
#include "Core.h"
#include "Utils/Color.h"
#include <map>
#include <functional>

namespace SoulEngine
{
	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& debugName, const std::string& vertexSource, const std::string& fragmentSource);

		Shader(const std::string& debugName, const std::string& vertexSource, const std::string& fragmentSoure);
		~Shader();

		/// <summary>
		/// Installs a program object as part of current rendering state
		/// </summary>
		void Use() const;

		/// <summary>
		/// Specify the value of a uniform variable for the current program object.
		/// </summary>
		/// <param name="location"></param>
		/// <param name="v"></param>
		void SetInt(const std::string& name, int val);

		/// <summary>
		/// Specify the value of a uniform variable for the current program object.
		/// </summary>
		/// <param name="location"></param>
		/// <param name="v"></param>
		void SetFloat(const std::string& name, float val);

		/// <summary>
		/// Specify the value of a uniform variable for the current program object.
		/// </summary>
		/// <param name="location"></param>
		/// <param name="v"></param>
		void SetColor(const std::string& name, const Color& val);

		/// <summary>
		/// Specify the value of a uniform variable for the current program object.
		/// </summary>
		/// <param name="location"></param>
		/// <param name="v"></param>
		void SetMatrix(const std::string& name, const float* val);

		/// <summary>
		/// Specify the value of a uniform variable for the current program object.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="val"></param>
		/// <param name="count"></param>
		void SetIntArray(const std::string& name, const int* val, int count);

		/// <summary>
		/// Specify the value of a uniform variable for the current program object.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="val"></param>
		/// <param name="count"></param>
		void SetFloatArray(const std::string& name, const float* val, int count);

		/// <summary>
		/// Specify the value of a uniform variable for the current program object.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="val"></param>
		/// <param name="count"></param>
		void SetColorArray(const std::string& name, const Color* val, int count);

		/// <summary>
		/// Specify the value of a uniform variable for the current program object.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="val"></param>
		/// <param name="count"></param>
		void SetMatrixArray(const std::string& name, const float* val, int count);

		/// <summary>
		/// Returns the location of a uniform variable
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		int GetUniformLocation(const std::string& name) const;

	private:
		unsigned int _id{ 0 };
		mutable std::map<std::string, int> _locations;

		void Compile(const std::string& debugName, int type, const std::string& source, const std::function<void()>& action);
	};
}