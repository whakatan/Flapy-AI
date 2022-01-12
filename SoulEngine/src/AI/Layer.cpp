#include "pch.h"
#include "AI/Layer.h"

namespace SoulEngine
{
	Layer::Layer(int neuronsCount, int weightsCount, const ActivationFunc& activation) :
		weights(weightsCount ? neuronsCount : 0), values(neuronsCount), bias(0.0f),
		_neuronsCount(neuronsCount), _weightsCount(weightsCount),
		_activation(activation)
	{
		for (auto& it : weights)
			it.resize(static_cast<size_t>(weightsCount));
	}

	void Layer::Predict(const std::vector<float>& inputs)
	{
		if (inputs.size() != _weightsCount)
			throw "";

		for (int i = 0; i < _neuronsCount; ++i)
		{
			float result = 0.0f;
			for (int j = 0; j < _weightsCount; ++j)
				result += weights[i][j] * inputs[j];
			values[i] = _activation(result + bias);
		}
	}

	void Layer::SetValues(const std::vector<float>& inputs)
	{
		for (int i = 0; i < _neuronsCount; ++i)
			values[i] = inputs[i];
	}
}