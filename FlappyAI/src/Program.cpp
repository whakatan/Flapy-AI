#include "Window/Application.h"
#include "Window.h"

using namespace SoulEngine;

int main()
{
	Application::Run(new Window());
	return 0;
}