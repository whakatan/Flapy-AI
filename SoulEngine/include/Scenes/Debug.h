#pragma once
#include "Core.h"
#include <glm/mat4x4.hpp>

namespace SoulEngine
{
	struct Color;
	class Texture;
	class Font;

	class Debug
	{
	public:
		static bool showCollider;

		static void DrawCircle(const glm::mat4& transform, const Color& color, float thickness, float fade);
		static void DrawLine(const glm::vec2& p0, const glm::vec2& p1, const Color& color);
		static void DrawRect(const glm::vec2& position, const glm::vec2& scale, const Color& color);
		static void DrawRect(const glm::vec4& raect, const Color& color);
		static void DrawRect(const glm::mat4& transform, const Color& color);
		static void DrawString(const std::string& text, const glm::vec2& position, const glm::vec2& scale, Ref<Font> font, const Color& color);

	protected:
		static void Init(int circleMaxInstance, int lineMaxInstance, int textMaxInstance);
		static void Destroy();

		static void Begin();
		static void BeginCircle();
		static void BeginLine();
		static void BeginText();

		static void Flush();
		static void FlushCircle();
		static void FlushLine();
		static void FlushText();

		friend class SceneManager;
	};
}