#include "BirdAI.h"
#include "GameManager.h"
#include "SoulEngine.h"
#include "Bird.h"
#include <sstream>

Scene* nscene;
Population* population;
float maxFitness = 0.0f;
int aliveCount = 0;

void BirdAI::Start()
{
	nscene = SceneManager::Active();
	population = new Population(50, 5, 0.1f, &BirdAI::CreateBird);
}

void BirdAI::Update()
{
	aliveCount = 0;
	float bestFitness = 0.0f;

	glm::vec2 pos1 = GameManager::instance->GetCurrentTop()->position;
	for (auto agent : population->GetCurrentPopulation())
		if (agent->isAlive)
			agent->Action();

	Agent* best = nullptr;
	for (auto agent : population->GetCurrentPopulation())
	{
		if (agent->isAlive)
		{
			best = agent;
			bestFitness = glm::max(agent->GetFitness(), bestFitness);
			aliveCount++;
		}
	}

	if (best)
		best->GetComponent<CircleRenderer>().color = Color::Blue();

	if (aliveCount == 0)
		NextGeneration();

	if (Input::GetKeyDown(Keys::KPAdd))
		NextGeneration();

	maxFitness = glm::max(maxFitness, bestFitness);
	std::stringstream s;
	s << "Generation: " << population->GetGeneration()
		<< "\nPopulation: " << population->GetPopulationSize()
		<< "\nAlive: " << aliveCount
		<< "\nBest Fitness: " << bestFitness
		<< "\nMax Fitness: " << maxFitness;

	GameManager::instance->text = s.str();
}

void BirdAI::NextGeneration()
{
	GameManager::instance->RestartPipes();
	aliveCount = population->GetPopulationSize();

	for (auto agent : population->GetCurrentPopulation())
	{
		agent->GetComponent<CircleRenderer>().visible = true;
		agent->GetTransform().position.y = 0.0f;
		auto& rigidbody = agent->GetComponent<Rigidbody2D>();
		rigidbody.SetAwake(true);
		rigidbody.Create();
		agent->isAlive = true;
	}
	population->NextGeneration();
}

Agent* BirdAI::CreateBird()
{
	auto gameObject = nscene->CreateGameObject("bird", GameManager::instance->birdX + 2.0f, 0.0f, 0.5f, 0.5f);
	auto& renderer = gameObject.AddComponent<CircleRenderer>();
	auto& rigidbody = gameObject.AddComponent<Rigidbody2D>();
	auto& collider = gameObject.AddComponent<CircleCollider2D>();
	gameObject.AddComponent<RaySensor2D>();
	collider.SetDensity(3.0f);
	renderer.color = { 0.8f, 0.6f, 0.2f, 1.0f };
	rigidbody.SetBullet(true);
	rigidbody.groupIndex = -1;
	rigidbody.SetBodyType(BodyType::Dynamic);
	rigidbody.SetFixedRotation(true);

	return &gameObject.AddComponent<Bird>();
}