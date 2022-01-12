#pragma once
#include "Core.h"
#include "Utils/Color.h"

#define GLM_FORCE_QUAT_DATA_XYZW
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

namespace SoulEngine
{
	class Font;
	class Texture;

	struct IgnoreSerialize
	{
		bool ignore = true;
	};

	struct Tag
	{
		std::string tag;

		Tag() = default;
		Tag(const Tag&) = default;
		Tag(std::string tag) : tag(std::move(tag)) {}

		bool operator ==(const Tag& tag)
		{
			return this->tag == tag.tag;
		}

		bool operator !=(const Tag& tag)
		{
			return this->tag != tag.tag;
		}

		operator std::string() const
		{
			return tag;
		}
	};

	struct Transform
	{
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 0.0f };
		glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };

		Transform() = default;
		Transform(float x, float y) : position(x, y, 0.0f) {}
		Transform(float x, float y, float z) : position(x, y, z) {}
		Transform(float x, float y, float scaleX, float scaleY) : position(x, y, 0.0f), scale(scaleX, scaleY, 0.0f) {}
		Transform(const glm::vec3& position) : position(position) {}
		Transform(const Transform&) = default;

		glm::mat4 GetTransform() const
		{
			const glm::mat4 identity(1.0f);
			return glm::translate(identity, position) * glm::toMat4(glm::quat(glm::radians(rotation))) * glm::scale(identity, scale);
		}
	};

	struct SpriteRenderer
	{
		bool visible{ true };
		Color color{ Color::White() };
		Ref<Texture> texture{ nullptr };
		glm::vec4 textureRect{ 0.0f, 0.0f, 1.0f, 1.0f };
		
		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const Color& color) : color(color) {}
	};

	struct CircleRenderer
	{
		bool visible{ true };
		Color color{ Color::White() };
		float thickness{ 1.0f };

		CircleRenderer() = default;
		CircleRenderer(const CircleRenderer&) = default;
		CircleRenderer(const Color& color) : color(color) {}
	};

	struct TextRenderer
	{
		bool visible{ true };
		Ref<Font> font{ nullptr };
		Color color{ Color::White() };
		std::string text{ "" };
	};
}