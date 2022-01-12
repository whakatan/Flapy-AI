#pragma once
#include <vector>

namespace SoulEngine
{
	using ActivationFunc = float(*)(float);
	struct Layer
	{
		Layer(int neuronsCount, int weightsCount, const ActivationFunc& activation);

		void Predict(const std::vector<float>& inputs);
		void SetValues(const std::vector<float>& inputs);

		int GetWeightsCount() const { return _weightsCount; }
		int GetNeuronsCount() const { return _neuronsCount; }

		float bias;
		std::vector<std::vector<float>> weights;
		std::vector<float> values;

	private:
		int _weightsCount;
		int _neuronsCount;
		ActivationFunc _activation;
	};
}