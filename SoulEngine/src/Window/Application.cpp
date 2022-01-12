#include "pch.h"
#include "Window/Application.h"
#include "Window/Input.h"
#include "Window/NativeWindow.h"
#include "Window/Time.h"
#include "Scenes/SceneManager.h"
#include <GLFW/glfw3.h>

namespace SoulEngine
{
    NativeWindow* Application::_window;

    void Application::Run(NativeWindow* window)
    {
        _window = window;
        Input::Start(window->_handle);

        _window->HandleCreated();
        SceneManager::Start();
        while (!glfwWindowShouldClose(window->_handle))
        {
            Time::SetDeltaTime();
            glfwPollEvents();

            window->PreRender();

            Input::Clear();
        }
        _window->HandleDestroying();
        delete _window;
    }

    void Application::Exit()
    {
        _window->Close();
        glfwTerminate();
    }
}
