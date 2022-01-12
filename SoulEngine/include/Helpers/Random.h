#pragma once
#include <random>
#include <limits>

namespace SoulEngine
{
	struct Random
	{
	public:
		static float Next();
		static float Next(float max);
		static float Next(float min, float max);

		template <typename T>
		static T Next()
		{
			return static_cast<T>(Next() * std::numeric_limits<T>::max());
		}

		template <typename T>
		static T Next(T max)
		{
			return static_cast<T>(Next(static_cast<float>(max)));
		}

		template <typename T>
		static T Next(T min, T max)
		{
			return static_cast<T>(Next(static_cast<float>(min), static_cast<float>(max)));
		}

	private:
		static std::uniform_real_distribution<float> _distribution;
		static std::random_device _device;
		static std::mt19937 _engine;
	};

}