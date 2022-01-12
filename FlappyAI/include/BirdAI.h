#pragma once
#include "Behavior.h"

using namespace SoulEngine;

class BirdAI : public Behavior
{
	void Start() override;
	void Update() override;
	void NextGeneration();

public:
	static Agent* CreateBird();
};