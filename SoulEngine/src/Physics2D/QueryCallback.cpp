#include "pch.h"
#include "Physics2D/QueryCallback.h"

namespace SoulEngine
{
	bool QueryCallback::ReportFixture(b2Fixture* fixture)
	{
		return false;
	}
}