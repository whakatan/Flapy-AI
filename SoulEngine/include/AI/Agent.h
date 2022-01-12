#pragma once
#include "Scenes/Entities/Behavior.h"
#include "AI/NeuralNetwork.h"
#include "AI/Chromosome.h"

namespace SoulEngine
{
	struct Transform;
	struct RaySensor2D;
	class GameObject;

	class Agent : public Behavior
	{
	public:
		float min{ -1.0 }, max{ 1.0 };
		bool isAlive{ true };
		std::vector<int> layers;

		RaySensor2D* sensor{ nullptr };
		Transform* transform{ nullptr };

		void AddFitness(float fitness);
		float GetFitness() const { return _fitness; };

		void Action();

		virtual void Initialize() { }
		virtual void Begin() { }

		virtual void OnAction(const std::vector<float>& inputs) {}

	private:
		float _fitness{ 0.0f };

		Chromosome _chromosome;
		NeuralNetwork _neuralNetwork;

		void UpdateChromosome(const Chromosome& chromosome);
		void Create() override;

		friend class Population;
	};
}