#include "pch.h"
#include "Helpers/Random.h"

namespace SoulEngine
{
	std::random_device Random::_device;
	std::mt19937 Random::_engine(_device());
	std::uniform_real_distribution<float> Random::_distribution(0.0f, 1.0f);
	
	float Random::Next()
	{
		return _distribution(_engine);
	}

	float Random::Next(float max)
	{
		return max * _distribution(_engine);
	}
	
	float Random::Next(float min, float max)
	{
		return (max - min) * _distribution(_engine) + min;
	}
}