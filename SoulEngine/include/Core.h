#pragma once
#ifndef CORE_H
#define CORE_H

#ifdef _WIN32
	#ifdef _WIN64
		#define NOMINMAX   
		#include <Windows.h>
		#define _PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#else
	#error "Unknown platform!"
#endif

#ifdef _DEBUG
	#if defined(_PLATFORM_WINDOWS)
		#define _DEBUGBREAK() __debugbreak()
	#else
		#error "Platform doesn't support debugbreak!"
	#endif
	#define _ENABLE_ASSERTS
#else
	#define _DEBUGBREAK()
#endif


#ifdef _ENABLE_ASSERTS
	#include <iostream>
	#define CORE_ASSERT(expr, msg) \
			if (!(expr)) {\
				std::cerr << msg << "\n"; \
				_DEBUGBREAK();\
		}
#else
	#define CORE_ASSERT(...)
#endif


#ifdef _PLATFORM_WINDOWS
	#include <string>
	const char PATH_SEPARATOR = '\\';

	void GetExecutableDirectory(std::string& buffer);
#else
	const char PATH_SEPARATOR = '/';

	void GetExecutableDirectory(char_t* buffer);
#endif

#include <memory>

namespace SoulEngine
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	constexpr Unique<T> CreateUnique(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}
#endif // !CORE_H