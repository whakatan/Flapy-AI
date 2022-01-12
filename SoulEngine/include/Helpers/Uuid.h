#pragma once
#include "Core.h"

namespace SoulEngine
{
	struct Uuid
	{
		Uuid();
		Uuid(uint64_t id);
		Uuid(const Uuid& id) = default;

		operator uint64_t() const { return _uuid; }
	private:
		uint64_t _uuid;
	};
}

namespace std
{
	template<>
	struct hash<SoulEngine::Uuid>
	{
		std::size_t operator() (const SoulEngine::Uuid& uuid) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};
}