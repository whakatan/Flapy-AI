#pragma once
#include "Behavior.h"

using namespace SoulEngine;

class PipeMovement : public Behavior
{
public:
	void DestroyAndCreate();

private:
	void Create() override;
	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	void CreatePipes();
	
	float deltaTime = 0.0f;

	GameObject pipe1;
	Rigidbody2D* rigidbody1{ nullptr };

	GameObject pipe2;
	Rigidbody2D* rigidbody2{ nullptr };
};