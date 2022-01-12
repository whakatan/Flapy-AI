#pragma once

namespace SoulEngine
{
    class NativeWindow;

    class Application
    {
    public:
        static void Run(NativeWindow* window);
        static void Exit();

    private:
        static NativeWindow* _window;
    };
}