#pragma once
#include "Core.h"
#include <glm/mat4x4.hpp>

namespace SoulEngine
{
	struct Color;
	class Texture;
	class Font;

	class UI
	{
	public:
		static void DrawCircle(const glm::mat4& transform, const Color& color, float thickness = 0.1f);
		static void DrawCircle(float x, float y, float radius, const Color& color, float thickness = 0.1f);
		static void FillCircle(float x, float y, float radius, const Color& color);

		static void DrawLine(const glm::vec2& p0, const glm::vec2& p1, const Color& color);
		static void DrawPolygon(const glm::vec2* points, size_t count, const Color& color);

		static void DrawRect(const glm::vec2& position, const glm::vec2& scale, const Color& color);
		static void DrawRect(const glm::vec4& rect, const Color& color);
		static void DrawRect(const glm::mat4& transform, const Color& color);

		static void FillPolygon(const glm::vec2* points, size_t count, const Color& color);
		static void FillRect(const glm::vec2& position, const glm::vec2& scale, const Color& color);
		static void FillRect(const glm::vec4& rect, const Color& color);
		static void FillRect(const glm::mat4& transform, const Color& color);

		static void DrawString(const std::string& text, const glm::vec2& position, const glm::vec2& scale, Ref<Font> font, const Color& color);

	protected:
		static void Init(int circleMaxInstance, int lineMaxInstance, int rectMaxInstance, int textMaxInstance);
		static void Fill(const glm::vec2& point, const Color& color);
		static void Destroy();

		static void Begin();
		static void BeginCircle();
		static void BeginLine();
		static void BeginRect();
		static void BeginText();

		static void Flush();
		static void FlushCircle();
		static void FlushLine();
		static void FlushRect();
		static void FlushText();

		friend class SceneManager;
	};
}