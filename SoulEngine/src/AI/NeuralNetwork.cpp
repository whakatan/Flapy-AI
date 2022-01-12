#include "pch.h"
#include "AI/NeuralNetwork.h"

namespace SoulEngine
{
	NeuralNetwork::NeuralNetwork() : _inputSize(0)
	{
	}

	NeuralNetwork::NeuralNetwork(int inputSize) : _inputSize(inputSize)
	{
	}

	NeuralNetwork::NeuralNetwork(std::vector<int>& layers) : _inputSize(layers[0])
	{
		for (auto layer : layers)
			AddLayer(layer);
	}

	int NeuralNetwork::WeightsCount()
	{
		int weightCount = static_cast<int>(layers.size());
		for (auto& layer : layers)
			weightCount += layer.GetNeuronsCount() * static_cast<int>(layer.weights.front().size());

		return weightCount;
	}

	void NeuralNetwork::AddLayer(int neuronsCount)
	{
		if (layers.empty()) layers.emplace_back(neuronsCount, 0.0f, &Sigmoid);
		else layers.emplace_back(neuronsCount, layers.back().GetNeuronsCount(), &Sigmoid);
	}

	std::vector<float>& NeuralNetwork::Prediction(const std::vector<float>& inputs)
	{
		if (inputs.size() == _inputSize)
		{
			layers.front().SetValues(inputs);
			size_t count = layers.size();
			for (size_t i = 1; i < count; ++i)
				layers[i].Predict(layers[i - 1].values);
		}

		return layers.back().values;
	}

	int NeuralNetwork::WeightsCount(std::vector<int>& layers)
	{
		int weightCount = static_cast<int>(layers.size());
		for (size_t i = 0; i < layers.size() - 1; ++i)
			weightCount += layers[i] * layers[i + 1];
		return weightCount;
	}

	float NeuralNetwork::Sigmoid(float x)
	{
		return 1.0f / (1.0f + expf(-x));
	}

	float NeuralNetwork::FastSigmoid(float x)
	{
		return x / (x + fabsf(x));
	}
}