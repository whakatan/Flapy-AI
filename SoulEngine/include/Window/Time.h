#pragma once
#include "Utils/Timestep.h"

namespace SoulEngine
{
    class Application;

    class Time
    {
    public:
        static Timestep GetTime();
        static Timestep GetDeltaTime() { return _deltaTime; }
        static Timestep GetFixedDeltaTime() { return _fixedDeltaTime; }

    private:
        static Timestep _deltaTime;
        static Timestep _lastFrameTime;
        static Timestep _fixedDeltaTime;

        static void SetDeltaTime();
        static void SetFixedDeltaTime();

        friend class Scene;
        friend class Physics2D;
        friend class Application;
    };
}