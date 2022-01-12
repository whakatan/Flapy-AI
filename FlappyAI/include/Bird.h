#pragma once
#include "Behavior.h"

using namespace SoulEngine;

class PlayerInput;
namespace SoulEngine
{
	struct Rigidbody2D;
}

class Bird : public Agent
{
public:
	void Jump();

private:
	void Initialize() override;
	void Update() override;
	void Begin() override;
	void OnAction(const std::vector<float>& inputs) override;
	void OnTriggerEnter(const Collision2D& collision) override;

	float jumpForce = 3.0f;

	Rigidbody2D* rigidbody2D{ nullptr };
};