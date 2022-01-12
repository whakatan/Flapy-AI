#pragma once
#include <vector>

namespace SoulEngine
{
	struct Chromosome
	{	
		Chromosome() = default;
		Chromosome(size_t size, float min, float max);

		void Initialize();

		float Get(int index) const;
		void Set(int index, const float& value);
		int GetCount() const;

		void Mutate(float mutationRate);
		static Chromosome Evolve(const Chromosome& chromosome, float evolveRate, float max);

		static Chromosome Crossover(const Chromosome& chromosome1, const Chromosome& chromosome2, int point);
		static Chromosome MakeChild(const Chromosome& chromosome1, const Chromosome& chromosome2, float mutationRate);

		bool operator==(const Chromosome& other) const;
		bool operator!=(const Chromosome& other) const;

	private:
		float _min{ 0.0f }, _max{ 0.0f };
		std::vector<float> _gens;

		friend class Population;
	};
}