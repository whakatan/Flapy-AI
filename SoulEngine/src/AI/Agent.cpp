#include "pch.h"
#include "AI/Agent.h"
#include "AI/Sensors/RaySensor2D.h"
#include "Physics2D/Physics2D.h"
#include "Scenes/UI.h"
#include "Scenes/Entities/GameObject.h"

#include <glm/vec2.hpp>

namespace SoulEngine
{
	void Agent::AddFitness(float fitness)
	{
		_fitness += fitness;
	}

	void Agent::Create()
	{
		if (!HasComponent<RaySensor2D>()) return;

		sensor = &GetComponent<RaySensor2D>();
		transform = &GetTransform();

		Initialize();

		layers.insert(layers.begin(), static_cast<float>(sensor->sensors.size()));

		_neuralNetwork = NeuralNetwork(layers);
		_chromosome = Chromosome(NeuralNetwork::WeightsCount(layers), min, max);
		_chromosome.Initialize();

		UpdateChromosome(_chromosome);
	}

	void Agent::Action()
	{
		signed short ignore = 0;
		if (HasComponent<Rigidbody2D>())
			ignore = GetComponent<Rigidbody2D>().groupIndex;

		std::vector<float> inputs;
		sensor->GetInputs(inputs, transform->position.x, transform->position.y, ignore);
		OnAction(_neuralNetwork.Prediction(inputs));
	}

	void Agent::UpdateChromosome(const Chromosome& chromosome)
	{
		Begin();

		_fitness = 0.0f;
		_chromosome = chromosome;

		int index = 0;
		for (Layer& layer : _neuralNetwork.layers)
		{
			const int weightsCount = layer.GetWeightsCount();
			if (weightsCount)
				layer.bias = chromosome.Get(index++);

			const int neuronsCount = layer.GetNeuronsCount();
			for (int i = 0; i < neuronsCount; ++i)
				for (int j = 0; j < weightsCount; ++j)
					layer.weights[i][j] = chromosome.Get(index++);
		}
	}
}