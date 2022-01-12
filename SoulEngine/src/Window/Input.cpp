#include "pch.h"
#include "Window/Input.h"

#include <GLFW/glfw3.h>

namespace SoulEngine
{
	constexpr int KEY_NULL = 3;

	uint16_t keyStates[GLFW_KEY_LAST];
	GLFWwindow* Input::_window;

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	bool Input::GetKey(Keys key)
	{
		auto state = glfwGetKey(_window, static_cast<int>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::GetKeyDown(Keys key)
 	{
		auto state = keyStates[static_cast<uint16_t>(key)] == GLFW_PRESS;
		return state;
	}

	bool Input::GetKeyUp(Keys key)
	{
		return keyStates[static_cast<uint16_t>(key)] == GLFW_RELEASE;
	}

	bool Input::GetMouseButton(MouseButtons button)
	{
		auto state = glfwGetMouseButton(_window, static_cast<uint16_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		double xpos, ypos;
		glfwGetCursorPos(_window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

	void Input::Start(GLFWwindow* window)
	{
		_window = window;
		for (int i = 0; i < GLFW_KEY_LAST; ++i) keyStates[i] = KEY_NULL;

		glfwSetKeyCallback(window, key_callback);
	}

	void Input::Clear()
	{
		for (int i = 0; i < GLFW_KEY_LAST; ++i)
			if (keyStates[i] != GLFW_REPEAT)
				keyStates[i] = KEY_NULL;
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		keyStates[key] = static_cast<uint16_t>(action);
	}
}