#pragma once
#include <vector>

namespace SoulEngine
{
	struct Sensor2D
	{
		float angle;
		float length;

		Sensor2D(float angle, float length) : angle(angle), length(length) { }
	};

	struct RaySensor2D
	{
		bool showSensors;
		std::vector<Sensor2D> sensors;
		void GetInputs(std::vector<float>& inputs, float x, float y, signed short ignoreIndex);
	};
}