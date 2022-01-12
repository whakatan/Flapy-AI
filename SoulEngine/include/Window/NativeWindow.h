#pragma once
#include <string>
#include <chrono>

typedef struct GLFWwindow GLFWwindow;

namespace SoulEngine
{
    class NativeWindow
    {
    public:
        NativeWindow();
        NativeWindow(const std::string& title);
        NativeWindow(const std::string& title, int width, int height, int major, int minor);
        virtual ~NativeWindow();

        void Show() const;
        void Hide() const;
        void Close();
        void SwapInterval(int interval) const;
        void FullScreen();
        void MakeCurrent() const;

        int GetFPS() const { return _fps; }
        int GetWidth() const { return _width; }
        int GetHeight() const { return _height; }

        void SetWidth(int width);
        void SetHeight(int height);
        void SetTitle(const std::string& title);

    protected:
        virtual void HandleCreated();
        virtual void HandleDestroying();
        virtual void OnResize();
        virtual void OnKeyDown();
        virtual void OnKeyUp();
        virtual void OnMouseMove();
        virtual void OnMouseUp();
        virtual void OnMouseEnter();
        virtual void OnMouseLeave();
        virtual void OnRender();

    private:
        std::chrono::steady_clock::time_point _lastTime;
        std::string _title;
        int _width, _height, _frame, _fps;
        GLFWwindow* _handle;

        void PreRender();
        void SetFrameInfo();
        void Init(const std::string& title, int width, int height, int major, int minor);

        friend class Application;
    };
}