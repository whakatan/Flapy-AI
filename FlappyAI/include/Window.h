#pragma once
#include "Window/NativeWindow.h"

class Window : public SoulEngine::NativeWindow
{
public:
	void HandleCreated() override;
};
