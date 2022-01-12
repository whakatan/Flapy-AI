#include "pch.h"
#include "Utils/Color.h"
#include <stdexcept>

namespace SoulEngine
{
	float& Color::operator[](int i)
	{
		if (i < 0 || i > 3)
			throw std::runtime_error("ArgumentOutOfRangeException");

		switch (i)
		{
		default:
		case 0: return _r;
		case 1: return _g;
		case 2: return _b;
		case 3: return _a;
		}
	}

	float const& Color::operator[](int i) const
	{
		if (i < 0 || i > 3)
			throw std::runtime_error("ArgumentOutOfRangeException");

		switch (i)
		{
		default:
		case 0: return _r;
		case 1: return _g;
		case 2: return _b;
		case 3: return _a;
		}
	}

	Color::Color(float r, float g, float b, float a) : _r(r), _g(g), _b(b), _a(a)
	{
	}
}