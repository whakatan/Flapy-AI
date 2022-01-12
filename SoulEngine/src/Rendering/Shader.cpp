#include "pch.h"
#include "Rendering/Shader.h"

#include <stdexcept>
#include <glad/gl.h>

namespace SoulEngine
{
	Ref<Shader> Shader::Create(const std::string& debugName, const std::string& vertexSource, const std::string& fragmentSource)
	{
		return CreateRef<Shader>(debugName, vertexSource, fragmentSource);
	}

	Shader::Shader(const std::string& debugName, const std::string& vertexSource, const std::string& fragmentSoure)
	{
		_id = glCreateProgram();
		Compile(debugName, GL_VERTEX_SHADER, vertexSource, [&]() {
			Compile(debugName, GL_FRAGMENT_SHADER, fragmentSoure, [&]() {
				glLinkProgram(_id);
				int length;
				glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
				if (length)
				{
					char* buffer = new char[length];
					glGetProgramInfoLog(_id, length, nullptr, buffer);
					throw std::runtime_error(debugName + ": " + buffer);
					delete[] buffer;
				}
			});
		});
	}

	Shader::~Shader()
	{
		glDeleteProgram(_id);
	}

	void Shader::Compile(const std::string& debugName, int type, const std::string& source, const std::function<void()>& action)
	{
		uint32_t shader = glCreateShader(type);
		const char* source_c = source.c_str();
		glShaderSource(shader, 1, &source_c, NULL);
		glCompileShader(shader);

		int length;
		std::string shaderType = type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader";
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		if (length)
		{
			char* buffer = new char[length];
			glGetShaderInfoLog(shader, length, nullptr, buffer);
			throw std::runtime_error("Name: " + debugName + ": " + "Shader Type: " + shaderType + ": " + buffer);
			delete[] buffer;
		}

		glAttachShader(_id, shader);
		action();
		glDeleteShader(shader);
	}
	
	void Shader::Use() const
	{
		glUseProgram(_id);
	}

	void Shader::SetInt(const std::string& name, int val)
	{
		glUniform1i(GetUniformLocation(name), val);
	}

	void Shader::SetFloat(const std::string& name, float val)
	{
		glUniform1f(GetUniformLocation(name), val);
	}

	void Shader::SetColor(const std::string& name, const Color& val)
	{
		glUniform4f(GetUniformLocation(name), val.R(), val.G(), val.B(), val.A());
	}
	
	void Shader::SetMatrix(const std::string& name, const float* val)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, val);
	}

	void Shader::SetIntArray(const std::string& name, const int* val, int count)
	{
		glUniform1iv(GetUniformLocation(name), count, val);
	}

	void Shader::SetFloatArray(const std::string& name, const float* val, int count)
	{
		glUniform1fv(GetUniformLocation(name), count, val);
	}

	void Shader::SetColorArray(const std::string& name, const Color* val, int count)
	{
		glUniform4fv(GetUniformLocation(name), count, val->Ptr());
	}

	void Shader::SetMatrixArray(const std::string& name, const float* val, int count)
	{
		glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, val);
	}

	int Shader::GetUniformLocation(const std::string& name) const
	{
		auto it = _locations.find(name);
		if (it != _locations.end()) return it->second;

		int location = glGetUniformLocation(_id, name.c_str());
		_locations.emplace(name, location);
		return location;
	}
}