#include "pch.h"
#include "AI/Chromosome.h"
#include "Helpers/Random.h"

#include <glm/glm.hpp>

namespace SoulEngine
{
	Chromosome::Chromosome(size_t size, float min, float max) : _gens(size), _min(min), _max(max)
	{
	}
	
	void Chromosome::Initialize()
	{
		size_t count = GetCount();
		for (int i = 0; i < count; ++i)
			_gens[i] = Random::Next(_min, _max);
	}
	
	float Chromosome::Get(int index) const
	{
		return _gens[index];
	}

	void Chromosome::Set(int index, const float& value)
	{
		_gens[index] = glm::clamp(_min, _max, value);
	}
	
	int Chromosome::GetCount() const
	{
		return static_cast<int>(_gens.size());
	}

	void Chromosome::Mutate(float mutationRate)
	{
		size_t count = GetCount();
		for (int i = 0; i < count; ++i)
			if (Random::Next() < mutationRate)
				_gens[i] = Random::Next(_min, _max);
	}

	Chromosome Chromosome::Evolve(const Chromosome& chromosome, float evolveRate, float max)
	{
		Chromosome child = chromosome;

		int count = chromosome.GetCount() - 1;
		for (int i = 0; i < count; ++i)
			if (Random::Next() < evolveRate)
				child._gens[i] += Random::Next(max);

		return child;
	}

	Chromosome Chromosome::MakeChild(const Chromosome& chromosome1, const Chromosome& chromosome2, float mutationRate)
	{
		int point = Random::Next<int>(chromosome1.GetCount());
		Chromosome child = Crossover(chromosome1, chromosome2, point);
		child.Mutate(mutationRate);
		return child;
	}

	Chromosome Chromosome::Crossover(const Chromosome& chromosome1, const Chromosome& chromosome2, int point)
	{
		size_t count = chromosome1.GetCount();
		Chromosome chromosome(count, chromosome1._min, chromosome1._max);

		for (int i = 0; i < point; ++i)
			chromosome._gens[i] = chromosome1._gens[i];

		for (int i = point; i < count; ++i)
			chromosome._gens[i] = chromosome2._gens[i];

		return chromosome;
	}

	bool Chromosome::operator==(const Chromosome& other) const
	{
		size_t count = GetCount();
		if (count != other.GetCount())
			return false;

		for (int i = 0; i < count; ++i)
			if (_gens[i] != other._gens[i])
				return false;

		return true;
	}

	bool Chromosome::operator!=(const Chromosome& other) const
	{
		return !(*this == other);
	}
}