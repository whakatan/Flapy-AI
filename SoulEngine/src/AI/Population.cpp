#include "pch.h"
#include "AI/Population.h"
#include "AI/Chromosome.h"
#include "AI/Agent.h"
#include "Helpers/Random.h"

namespace SoulEngine
{
	Population::Population(int populationSize, int elitism, float mutationRate, const CreateAgent& createAgent) :
		_population(2), _elitism(elitism), _mutationRate(mutationRate), fitnessIndex(0), _createAgent(createAgent), _generation(1), _evolveCount(0)
	{
		_population[0].resize(populationSize);
		_population[1].resize(populationSize);

		for (int i = 0; i < populationSize; ++i)
			_population[0][i] = _createAgent();
	}

	void Population::NextGeneration()
	{
		_evolveCount = 0;
		std::vector<Agent*>& current = _population[_current];
		std::vector<Agent*>& next = _population[!_current];

		CalcFitness(current);
		std::sort(current.begin(), current.end(), [&](const Agent* a1, const Agent* a2) { return a1->_fitness > a2->_fitness; });
		
		size_t count = current.size();
		for (int i = 0; i < count; ++i)
		{
			if (i < _elitism) next[i] = current[i];
			else
			{
				const Agent* parent1 = RouletteWheelSelection(current);
				const Agent* parent2 = RouletteWheelSelection(current);

				if (parent1->_chromosome == parent2->_chromosome)
				{
					_evolveCount++;
					current[i]->UpdateChromosome(Chromosome::Evolve(parent1->_chromosome, _mutationRate, 0.1f));
				}
				else
					current[i]->UpdateChromosome(Chromosome::MakeChild(parent1->_chromosome, parent2->_chromosome, _mutationRate));
				next[i] = current[i];
			}
			current[i]->_fitness = 0.0f;
		}
		Swap();
		_generation++;
	}
	
	void Population::CalcFitness(const std::vector<Agent*>& population)
	{
		sumFitness = 0.0f;
		size_t count = population.size();
		for (size_t i = 0; i < count; ++i)
			sumFitness += population[i]->_fitness;
	}

	std::vector<Agent*>& Population::GetCurrentPopulation()
	{
		return _population[_current];
	}

	std::vector<Agent*>& Population::GetNextPopulation()
	{
		return _population[!_current];
	}

	void Population::Swap()
	{
		_current = !_current;
	}

	const Agent* Population::RouletteWheelSelection(const std::vector<Agent*>& population)
	{
		float probability = 0.0f;
		float rnd = Random::Next();

		int count = static_cast<int>(population.size()) - 1;
		for (int i = count; i >= 0; --i)
		{
			probability += population[i]->_fitness / sumFitness;
			if (rnd < probability)
				return population[i];
		}
		return population[0];
	}
}