#pragma once
#include "Core.h"
#include <glm/mat4x4.hpp>

namespace SoulEngine
{
	struct Transform;
	struct SpriteRenderer;
	struct CircleRenderer;
	struct TextRenderer;
	struct Color;
	class Font;
	class Texture;

	class Renderer2D
	{
	public:
		Renderer2D(int spriteMaxInstance, int circleMaxInstance, int lineMaxInstance, int textMaxInstance);
		~Renderer2D();

		void DrawSprite(const Transform& transform, const SpriteRenderer& renderer) const;
		void DrawCircle(const Transform& transform, const CircleRenderer& renderer) const;
		void DrawCircle(const glm::mat4& transform, const Color& color, float thickness) const;
		void DrawLine(const glm::vec2& p0, const glm::vec2& p1, const Color& color) const;
		void DrawRect(const glm::vec2& position, const glm::vec2& scale, const Color& color) const;
		void DrawRect(const glm::vec4& rect, const Color& color) const;
		void DrawRect(const glm::mat4& transform, const Color& color) const;
		void DrawString(const Transform& transform, const TextRenderer& renderer) const;
		void DrawString(const std::string& text, const glm::vec2& position, const glm::vec2& scale, Ref<Font> font, const Color& color) const;

	protected:
		int SetTextures(Ref<Texture> texture) const;

		void Begin() const;
		void BeginSprite() const;
		void BeginCircle() const;
		void BeginLine() const;
		void BeginText() const;

		void Flush() const;
		void FlushSprite() const;
		void FlushCircle() const;
		void FlushLine() const;
		void FlushText() const;
		
		friend class RendererApi;
	};
}