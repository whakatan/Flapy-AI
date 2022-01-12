#include "pch.h"
#include "Helpers/Uuid.h"
#include "Helpers/Random.h"

#include <limits>

namespace SoulEngine
{
	Uuid::Uuid() : _uuid(Random::Next<int>())
	{
	}

	Uuid::Uuid(uint64_t id)
	{
		_uuid = id;
	}
}