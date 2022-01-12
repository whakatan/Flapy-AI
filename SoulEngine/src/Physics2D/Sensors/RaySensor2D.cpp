#include "pch.h"
#include "AI/Sensors/RaySensor2D.h"
#include "Physics2D/Physics2D.h"
#include "Scenes/UI.h"
#include <glm/glm.hpp>

namespace SoulEngine
{
	void RaySensor2D::GetInputs(std::vector<float>& inputs, float x, float y, signed short ignoreIndex)
	{
		int count = static_cast<int>(sensors.size());

		glm::vec2 pos = glm::vec2(x, y);
		glm::vec2 apos = glm::abs(glm::vec2(x, y));
		for (int i = 0; i < count; ++i)
		{
			auto& sen = sensors[i];
			float angle = glm::radians(sen.angle);
			glm::vec2 rotated = { glm::cos(angle), glm::sin(angle) };
			glm::vec2 newPoint = pos + glm::vec2(rotated.x * sen.length, rotated.y * sen.length);
			auto hit = Physics2D::RayCast(pos, newPoint, ignoreIndex);
			if (hit)
			{
				if (showSensors)
					UI::DrawLine(pos, hit.point, Color::Red());
				inputs.emplace_back(glm::length(hit.point - pos));
			}
			else
			{
				if (showSensors)
					UI::DrawLine(pos, newPoint, Color::Red());
				inputs.emplace_back(glm::length(newPoint - pos));
			}
		}
	}
}