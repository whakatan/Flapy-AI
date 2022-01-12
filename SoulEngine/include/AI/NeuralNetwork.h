#pragma once
#include "AI/Layer.h"

namespace SoulEngine
{
	struct NeuralNetwork
	{
	public:
		NeuralNetwork();
		NeuralNetwork(int inputSize);
		NeuralNetwork(std::vector<int>& layers);

		int WeightsCount();
		void AddLayer(int neuronsCount);
		std::vector<float>& Prediction(const std::vector<float>& inputs);

		static int WeightsCount(std::vector<int>& layers);
		static float Sigmoid(float x);
		static float FastSigmoid(float x);

		std::vector<Layer> layers;
	private:
		int _inputSize;

		friend class Agent;
	};
}