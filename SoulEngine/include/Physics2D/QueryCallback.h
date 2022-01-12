#pragma once

#include <box2d/b2_world_callbacks.h>

namespace SoulEngine
{
	class QueryCallback : public b2QueryCallback
	{
		virtual bool ReportFixture(b2Fixture* fixture);
	};
}