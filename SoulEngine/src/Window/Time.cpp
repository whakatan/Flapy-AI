#include "pch.h"
#include "Window/Time.h"
#include <chrono>
#include <GLFW/glfw3.h>

namespace SoulEngine
{
	using clock_t = std::chrono::high_resolution_clock;

	Timestep Time::_deltaTime;
	Timestep Time::_lastFrameTime;

	Timestep Time::_fixedDeltaTime;

	clock_t::time_point _lastFixedClock = clock_t::now();

	Timestep Time::GetTime()
	{
		return static_cast<float>(glfwGetTime());
	}

	void Time::SetDeltaTime()
	{
		float time = static_cast<float>(glfwGetTime());
		_deltaTime = time - _lastFrameTime;
		_lastFrameTime = time;
	}

	void Time::SetFixedDeltaTime()
	{
		clock_t::time_point now = clock_t::now();
		_fixedDeltaTime = std::chrono::duration<float>(now - _lastFixedClock).count();
		_lastFixedClock = now;
	}
}