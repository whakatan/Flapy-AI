#include "pch.h"
#include "Window/NativeWindow.h"
#include "Scenes/SceneManager.h"
#include "Rendering/RendererApi.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

using namespace std::chrono_literals;

namespace SoulEngine
{
    const int _cWIDTH = 1280;
    const int _cHEIGHT = 720;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void character_callback(GLFWwindow* window, unsigned int codepoint);
    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    NativeWindow::NativeWindow()
    {
        Init("", _cWIDTH, _cHEIGHT, 4, 6);
    }

    NativeWindow::NativeWindow(const std::string& title)
    {
        Init(title, _cWIDTH, _cHEIGHT, 4, 6);
    }

    NativeWindow::NativeWindow(const std::string& title, int width, int height, int major, int minor)
    {
        Init(title, width, height, major, minor);
    }

    NativeWindow::~NativeWindow()
    {
        SceneManager::Destroy();

        glfwDestroyWindow(_handle);
    }

    void NativeWindow::Show() const
    {
        glfwShowWindow(_handle);
    }

    void NativeWindow::Hide() const
    {
        glfwHideWindow(_handle);
    }

    void NativeWindow::SetWidth(int width)
    {
        _width = width;
        glfwSetWindowSize(_handle, _width, _height);
        OnResize();
    }

    void NativeWindow::SetHeight(int height)
    {
        _height = height;
        glfwSetWindowSize(_handle, _width, _height);
        OnResize();
    }

    void NativeWindow::SetTitle(const std::string& title)
    {
        _title = title;
        glfwSetWindowTitle(_handle, title.c_str());
    }

    void NativeWindow::HandleCreated()
    {
    }

    void NativeWindow::HandleDestroying()
    {
    }

    void NativeWindow::OnKeyDown()
    {
    }

    void NativeWindow::OnKeyUp()
    {
    }

    void NativeWindow::OnMouseMove()
    {
    }

    void NativeWindow::OnMouseUp()
    {
    }

    void NativeWindow::OnMouseEnter()
    {
    }

    void NativeWindow::OnMouseLeave()
    {
    }

    void NativeWindow::OnRender()
    {

    }

    void NativeWindow::OnResize()
    {
    }

    void NativeWindow::Close()
    {
        HandleDestroying();
        glfwDestroyWindow(_handle);
        glfwTerminate();
    }

    void NativeWindow::SwapInterval(int interval) const
    {
        glfwSwapInterval(interval);
    }

    void NativeWindow::FullScreen()
    {
    }

    void NativeWindow::MakeCurrent() const
    {
        glfwMakeContextCurrent(_handle);
    }

    void NativeWindow::PreRender()
    {
        SceneManager::Begin();
        SceneManager::Update();
        OnRender();
        SceneManager::Flush();

        glfwSwapBuffers(_handle);
        SetFrameInfo();
    }

    void NativeWindow::SetFrameInfo()
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastTime).count();
        if (duration >= 1000)
        {
            _fps = _frame;
            _lastTime = now;
            _frame = 0;
        }
        _frame++;
    }

    void NativeWindow::Init(const std::string& title, int width, int height, int major, int minor)
    {
        glfwInit();

        _title = title; _width = width; _height = height;
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        RendererApi::SetMultisample(4);
        RendererApi::Resize(_cWIDTH, _cHEIGHT);
        _handle = glfwCreateWindow(_cWIDTH, _cHEIGHT, title.c_str(), NULL, NULL);
        
        //RendererApi::Resize(mode->width, mode->height);
        //_handle = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, NULL);
        //glfwSetWindowMonitor(_handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        
        glfwMakeContextCurrent(_handle);
        gladLoadGL(glfwGetProcAddress);
        
        glfwSetFramebufferSizeCallback(_handle, framebuffer_size_callback);
        glfwSetCharCallback(_handle, character_callback);
        glfwSetCursorPosCallback(_handle, cursor_position_callback);
        glfwSetCursorEnterCallback(_handle, cursor_enter_callback);
        glfwSetMouseButtonCallback(_handle, mouse_button_callback);
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        RendererApi::Resize(width, height);
    }

    void character_callback(GLFWwindow* window, unsigned int codepoint)
    {
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
    }

    void cursor_enter_callback(GLFWwindow* window, int entered)
    {

    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {

    }
}