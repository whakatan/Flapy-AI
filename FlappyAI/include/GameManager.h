#pragma once
#include "Behavior.h"
#include <string>

class PipeMovement;
using namespace SoulEngine;

class GameManager : public Behavior
{
public:
	static GameManager* instance;

	void SetLastTop(Transform& transform);
	void SetLastBottom(Transform& transform);
	void RestartPipes();

	const Transform* GetLastTop() const { return _lastTop; }
	const Transform* GetLastBottom() const { return _lastBottom; }

	const Transform* GetCurrentTop() const { return _currentTop; }
	const Transform* GetCurrentBottom() const { return _currentBottom; }

	std::string text;
	float birdX{ 0.0f };
	float pipeGapSize = 3.0f;
	float pipeMinSize = 3.0f;
	float pipeWait = 4.0f;
	float pipeSpeed = -5.0f;

	std::vector<PipeMovement*> pipes;

private:
	Transform* _lastTop{ nullptr };
	Transform* _lastBottom{ nullptr };
	Transform* _currentTop{ nullptr };
	Transform* _currentBottom{ nullptr };

	void Create() override;
	void Update() override;
};