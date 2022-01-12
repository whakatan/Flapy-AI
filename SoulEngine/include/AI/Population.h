#pragma once

namespace SoulEngine
{
	class Agent;
	struct Chromosome;
	using CreateAgent = Agent*(*)();

	class Population
	{
	public:
		Population(int populationSize, int elitism, float mutationRate, const CreateAgent& createAgent);
		void NextGeneration();
		void CalcFitness(const std::vector<Agent*>& population);
		std::vector<Agent*>& GetCurrentPopulation();
		std::vector<Agent*>& GetNextPopulation();
		void Swap();

		const Agent* RouletteWheelSelection(const std::vector<Agent*>& population);

		float GetAvarageFitness() const { return sumFitness / static_cast<float>(_population[0].size()); }
		int GetPopulationSize() const { return static_cast<int>(_population[0].size()); }
		int GetGeneration() const { return _generation; }
		int GetEvolveCount() const { return _evolveCount; }
		int GetElitism() const { return _elitism; }
	private:
		int _evolveCount;
		int _generation;
		int _elitism;
		float _mutationRate;
		CreateAgent _createAgent;

		size_t _current{ 0 };
		std::vector<std::vector<Agent*>> _population;

		size_t fitnessIndex;
		float sumFitness{ 0.0f };
	};
}