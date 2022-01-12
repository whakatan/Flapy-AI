#pragma once

namespace SoulEngine
{
	struct Color
	{
	public:
		inline static const Color Empty() { return { 0.0f, 0.0f, 0.0f, 0.0f }; }
		inline static const Color Black() { return { 0.0f, 0.0f, 0.0f, 1.0f }; }
		inline static const Color White() { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
		inline static const Color Red() { return { 1.0f, 0.0f, 0.0f, 1.0f }; }
		inline static const Color Green() { return { 0.0f, 1.0f, 0.0f, 1.0f }; }
		inline static const Color Blue() { return { 0.0f, 0.0f, 1.0f, 1.0f }; }
		inline static const Color Grey() { return { 0.5f, 0.5f, 0.5f, 1.0f }; }

		Color() : Color(0.0f, 0.0f, 0.0f, 0.0f) { }
		Color(float r, float g, float b, float a);

		float R() const { return _r; }
		void R(float r) { if (r > 1) _r = 1; else if (r < 0) _r = 0; }
		
		float G() const { return _g; }
		void G(float g) { if (g > 1) _g = 1; else if (g < 0) _g = 0; }

		float B() const { return _b; }
		void B(float b) { if (b > 1) _b = 1; else if (b < 0) _b = 0; }

		float A() const { return _a; }
		void A(float a) { if (a > 1) _a = 1; else if (a < 0) _a = 0; }

		float& operator[](int i);
		float const& operator[](int i) const;

		inline static Color Lerp(const Color& c1, const Color& c2, float amount)
		{
			return Color(c1._r + (c2._r - c1._r) * amount,
				c1._g + (c2._g - c1._g) * amount, c1._b + (c2._b - c1._b) * amount,
				c1._a + (c2._a - c1._a) * amount);
		}

		inline float GetSaturation()
		{
			float s = 0;
			float max = _r, min = _r;

			if (_g > max) max = _g;
			if (_b > max) max = _b;

			if (_g < min) min = _g;
			if (_b < min) min = _b;

			if (max != min)
			{
				float l = (max + min) / 2.0f;

				if (l <= 0.5f) s = (max - min) / (max + min);
				else s = (max - min) / (2.0f - max - min);
			}
			return s;
		}

		inline float GetHue()
		{
			if (_r == _g && _r == _b)
				return 0;

			float hue = 0.0f;

			float max = _r, min = _r;

			if (_g > max) max = _g;
			if (_b > max) max = _b;
			if (_g < min) min = _g;
			if (_b < min) min = _b;

			float delta = max - min;

			if (_r == max) hue = (_g - _b) / delta;
			else if (_g == max) hue = 2.0f + (_b - _r) / delta;
			else if (_b == max) hue = 4.0f + (_r - _g) / delta;

			hue *= 60.0f;

			if (hue < 0.0f) hue += 360.0f;
			return hue;
		}

		inline float GetBrightness()
		{
			float max = _r, min = _r;

			if (_g > max) max = _g;
			if (_b > max) max = _b;

			if (_g < min) min = _g;
			if (_b < min) min = _b;

			return (max + min) / 2.0f;
		}

		inline const float* Ptr() const
		{
			return &_r;
		}

		inline uint32_t Zipped() const
		{
			return (uint32_t)((int)(_r * 255) << 24) | (uint32_t)((int)(_g * 255) << 16) | (uint32_t)((int)(_b * 255) << 8) | (uint32_t)(_a * 255);
		}

	private:
		float _r;
		float _g;
		float _b;
		float _a;
	};
}