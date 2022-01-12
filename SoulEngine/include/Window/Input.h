#pragma once

#include "Inputs.h"
#include <glm/vec2.hpp>

typedef struct GLFWwindow GLFWwindow;

namespace SoulEngine
{
	class Input
	{
	public:
		static bool GetKey(Keys key);
		static bool GetKeyDown(Keys key);
		static bool GetKeyUp(Keys key);

		static bool GetMouseButton(MouseButtons button);
		static bool GetMouseButtonDown(MouseButtons button);
		static bool GetMouseButtonUp(MouseButtons button);

		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	private:
		static void Start(GLFWwindow* window);
		static void Clear();

		static GLFWwindow* _window;
		friend class Application;
	};
}