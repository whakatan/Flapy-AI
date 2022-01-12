#include "pch.h"
#include "Scenes/Renderer2D.h"
#include "Scenes/SceneManager.h"
#include "Scenes/MainCamera.h"
#include "Assets/Shaders/ShaderManager.h"
#include "Assets/Fonts/Font.h"
#include "Scenes/Entities/Components.h"
#include "Rendering/RendererApi.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/VertexArray.h"
#include "Rendering/UniformBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Assets/Fonts/FontManager.h"

namespace SoulEngine
{
	struct SpriteInstance
	{
		glm::mat4 transform;
		glm::vec4 textureRect;
		int textureIndex;
		uint32_t packedColor;
		glm::vec2 pad0;
	};

	struct CircleInstance
	{
		glm::mat4 transform;
		uint32_t packedColor;
		float thickness;
		glm::vec2 pad0;
	};

	struct LineInstance
	{
		glm::vec2 position;
		uint32_t packedColor;
		float pad0;
	};

	struct TextInstance
	{
		glm::vec4 vertex;
		glm::vec4 color;
	};

	struct RendererData
	{
		int maxTextureUnits{ 0 };

		Ref<Texture>* textures{ nullptr };
		int textureCount { 0 };

		Ref<IndexBuffer> indexBuffer{ nullptr };

		// sprite
		int spriteIndex{ 0 };
		int spriteMaxInstance{ 0 };
		SpriteInstance* spriteInstances{ nullptr };
		SpriteInstance* spriteBeginInstances{ nullptr };
		Ref<Shader> spriteShader{ nullptr };
		Ref<VertexArray> spriteVertexArray{ nullptr };
		Ref<UniformBuffer> spriteInstanceBuffer{ nullptr };

		// circle
		int circleIndex{ 0 };
		int circleMaxInstance{ 0 };
		CircleInstance* circleInstances{ nullptr };
		CircleInstance* circleBeginInstances{ nullptr };
		Ref<Shader> circleShader{ nullptr };
		Ref<VertexArray> circleVertexArray{ nullptr };
		Ref<UniformBuffer> circleInstanceBuffer{ nullptr };

		// line
		int lineIndex{ 0 };
		int lineMaxInstance{ 0 };
		LineInstance* lineInstances{ nullptr };
		LineInstance* lineBeginInstances{ nullptr };
		Ref<Shader> lineShader{ nullptr };
		Ref<VertexArray> lineVertexArray{ nullptr };
		Ref<UniformBuffer> lineInstanceBuffer{ nullptr };

		// line
		int textIndex{ 0 };
		int textMaxInstance{ 0 };
		Ref<Font> lastFont{ nullptr };
		TextInstance* textInstances{ nullptr };
		TextInstance* textBeginInstances{ nullptr };
		Ref<Shader> textShader{ nullptr };
		Ref<VertexArray> textVertexArray{ nullptr };
		Ref<UniformBuffer> textInstanceBuffer{ nullptr };
	};

	static RendererData rendererData;

	Renderer2D::Renderer2D(int spriteMaxInstance, int circleMaxInstance, int lineMaxInstance, int textMaxInstance)
	{
		rendererData.maxTextureUnits = RendererApi::GetMaxTextureUnits();
		rendererData.textures = new Ref<Texture>[rendererData.maxTextureUnits];

		byte data[4]{ 255, 255, 255, 255 };
		rendererData.textures[0] = Texture::Create(&data, 1, 1);
		rendererData.textureCount = 1;

		byte* indices = new byte[6]{ 0, 1, 2, 2, 1, 3 };

		// sprite
		rendererData.spriteIndex = 0;
		rendererData.spriteMaxInstance = spriteMaxInstance;
		rendererData.spriteBeginInstances = new SpriteInstance[spriteMaxInstance];
		rendererData.spriteShader = ShaderManager::Get("Sprite");
		rendererData.spriteVertexArray = VertexArray::Create();
		rendererData.spriteInstanceBuffer = UniformBuffer::Create(Buffers::Structured, spriteMaxInstance, sizeof(SpriteInstance));
		rendererData.indexBuffer = IndexBuffer::Create(indices, sizeof(byte) * 6, IndexType::Byte);
		
		// circle
		rendererData.circleIndex = 0;
		rendererData.circleMaxInstance = circleMaxInstance;
		rendererData.circleBeginInstances = new CircleInstance[circleMaxInstance];
		rendererData.circleShader = ShaderManager::Get("Circle");
		rendererData.circleVertexArray = VertexArray::Create();
		rendererData.circleInstanceBuffer = UniformBuffer::Create(Buffers::Structured, circleMaxInstance, sizeof(CircleInstance));
		rendererData.indexBuffer->Bind();

		// line
		rendererData.lineIndex = 0;
		rendererData.lineMaxInstance = lineMaxInstance;
		rendererData.lineBeginInstances = new LineInstance[lineMaxInstance];
		rendererData.lineShader = ShaderManager::Get("Line");
		rendererData.lineVertexArray = VertexArray::Create();
		rendererData.lineInstanceBuffer = UniformBuffer::Create(Buffers::Structured, lineMaxInstance, sizeof(LineInstance));

		// text
		rendererData.textIndex = 0;
		rendererData.textMaxInstance = textMaxInstance;
		rendererData.textBeginInstances = new TextInstance[textMaxInstance];
		rendererData.textShader = ShaderManager::Get("Text");
		rendererData.textVertexArray = VertexArray::Create();
		rendererData.textInstanceBuffer = UniformBuffer::Create(Buffers::Structured, textMaxInstance, sizeof(TextInstance));
	}

	Renderer2D::~Renderer2D()
	{
		delete[] rendererData.textures;
		delete[] rendererData.spriteBeginInstances;
		delete[] rendererData.circleBeginInstances;
		delete[] rendererData.lineBeginInstances;
		delete[] rendererData.textBeginInstances;
	}

	void Renderer2D::Begin() const
	{
		BeginSprite();
		BeginCircle();
		BeginLine();
		BeginText();
	}

	void Renderer2D::DrawSprite(const Transform& transform, const SpriteRenderer& renderer) const
	{
		if (rendererData.spriteIndex >= rendererData.spriteMaxInstance)
		{
			FlushSprite();
			BeginSprite();
		}

		int index = SetTextures(renderer.texture);

		rendererData.spriteInstances->transform = transform.GetTransform();
		rendererData.spriteInstances->textureIndex = index;
		rendererData.spriteInstances->packedColor = renderer.color.Zipped();
		rendererData.spriteInstances->textureRect = renderer.textureRect;
		rendererData.spriteInstances++;
		rendererData.spriteIndex++;
	}

	void Renderer2D::DrawCircle(const Transform& transform, const CircleRenderer& renderer) const
	{
		DrawCircle(transform.GetTransform(), renderer.color, renderer.thickness);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const Color& color, float thickness) const
	{
		if (rendererData.circleIndex >= rendererData.circleMaxInstance)
		{
			FlushCircle();
			BeginCircle();
		}

		rendererData.circleInstances->transform = transform;
		rendererData.circleInstances->packedColor = color.Zipped();
		rendererData.circleInstances->thickness = thickness;
		rendererData.circleInstances++;
		rendererData.circleIndex++;
	}

	void Renderer2D::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const Color& color) const
	{
		if (rendererData.lineIndex >= rendererData.lineMaxInstance)
		{
			FlushLine();
			BeginLine();
		}

		auto c = color.Zipped();
		rendererData.lineInstances->position = p0;
		rendererData.lineInstances->packedColor = c;
		rendererData.lineInstances++;

		rendererData.lineInstances->position = p1;
		rendererData.lineInstances->packedColor = c;
		rendererData.lineInstances++;

		rendererData.lineIndex += 2;
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const Color& color) const
	{
		static const glm::vec4 vertices[4] = { glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.0f, 1.0f) };

		DrawLine(transform * vertices[0], transform * vertices[1], color);
		DrawLine(transform * vertices[1], transform * vertices[3], color);
		DrawLine(transform * vertices[3], transform * vertices[2], color);
		DrawLine(transform * vertices[2], transform * vertices[0], color);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const Color& color) const
	{
		DrawRect({ position, size }, color);
	}

	void Renderer2D::DrawRect(const glm::vec4& rect, const Color& color) const
	{
		glm::vec2 leftTop{ rect.x - rect.z * 0.5f, rect.y - rect.w * 0.5f };
		glm::vec2 rightTop{ rect.x + rect.z * 0.5f, rect.y - rect.w * 0.5f };
		glm::vec2 leftBottom{ rect.x - rect.z * 0.5f, rect.y + rect.w * 0.5f };
		glm::vec2 rightBottom{ rect.x + rect.z * 0.5f, rect.y + rect.w * 0.5f };

		DrawLine(leftBottom, rightBottom, color);
		DrawLine(rightBottom, rightTop, color);
		DrawLine(rightTop, leftTop, color);
		DrawLine(leftTop, leftBottom, color);
	}

	void Renderer2D::DrawString(const Transform& transform, const TextRenderer& renderer) const
	{
		DrawString(renderer.text, transform.position, transform.scale, renderer.font, renderer.color);
	}

	void Renderer2D::DrawString(const std::string& text, const glm::vec2& position, const glm::vec2& scale, Ref<Font> font, const Color& color) const
	{
		if (font == nullptr)
			return;

		if (rendererData.textIndex >= rendererData.textMaxInstance || (rendererData.lastFont && rendererData.lastFont != font))
		{
			FlushText();
			BeginText();
		}
		rendererData.lastFont = font;

		auto camera = SceneManager::Active()->GetCamera();

		float sizeH = camera->GetSize() * (1.0f / camera->GetZoom());
		float sizeW = camera->GetAspect() * sizeH;

		float aspectX = sizeW / RendererApi::GetWidth();
		float aspectY = sizeH / RendererApi::GetHeight();

		glm::vec4 clr = { color.R(), color.G(), color.B(), color.A() };

		float x = position.x;
		float y = 0.0f;
		for (const char& c : text)
		{
			if (rendererData.textIndex >= rendererData.textMaxInstance)
			{
				FlushText();
				BeginText();

				rendererData.lastFont = font;
			}

			if (c == '\n')
			{
				x = position.x;
				y += rendererData.textBeginInstances->vertex.w * scale.y;
			}

			Character* character = font->characters[c];

			const glm::vec4& rect = character->TextureRect;

			float xpos = x + (character->Bearing.x * scale.x) * aspectX;
			float ypos = position.y - ((character->Size.y - character->Bearing.y) * scale.y) * aspectY - y;

			float w = character->Size.x * scale.x * aspectX;
			float h = character->Size.y * scale.y * aspectY;

			rendererData.textInstances->vertex = { xpos, ypos, rect.x, rect.y + rect.w };
			rendererData.textInstances->color = clr;
			rendererData.textInstances++;

			rendererData.textInstances->vertex = { xpos + w, ypos, rect.x + rect.z, rect.y + rect.w };
			rendererData.textInstances->color = clr;
			rendererData.textInstances++;

			rendererData.textInstances->vertex = { xpos, ypos + h, rect.x, rect.y };
			rendererData.textInstances->color = clr;
			rendererData.textInstances++;

			rendererData.textInstances->vertex = { xpos, ypos + h, rect.x, rect.y };
			rendererData.textInstances->color = clr;
			rendererData.textInstances++;

			rendererData.textInstances->vertex = { xpos + w, ypos, rect.x + rect.z, rect.y + rect.w };
			rendererData.textInstances->color = clr;
			rendererData.textInstances++;

			rendererData.textInstances->vertex = { xpos + w, ypos + h, rect.x + rect.z, rect.y };
			rendererData.textInstances->color = clr;
			rendererData.textInstances++;

			x += character->Advance.x * scale.x * aspectX;
			rendererData.textIndex += 6;
		}
	}

	void Renderer2D::Flush() const
	{
		if (rendererData.spriteIndex) FlushSprite();
		if (rendererData.circleIndex) FlushCircle();
		if (rendererData.lineIndex) FlushLine();
		if (rendererData.textIndex) FlushText();
	}

	void Renderer2D::BeginSprite() const
	{
		rendererData.textureCount = 1;

		rendererData.spriteIndex = 0;
		rendererData.spriteInstances = rendererData.spriteBeginInstances;
	}

	void Renderer2D::BeginCircle() const
	{
		rendererData.circleIndex = 0;
		rendererData.circleInstances = rendererData.circleBeginInstances;
	}

	void Renderer2D::BeginLine() const
	{
		rendererData.lineIndex = 0;
		rendererData.lineInstances = rendererData.lineBeginInstances;
	}

	void Renderer2D::BeginText() const
	{
		rendererData.lastFont = nullptr;
		rendererData.textIndex = 0;
		rendererData.textInstances = rendererData.textBeginInstances;
	}

	void Renderer2D::FlushSprite() const
	{
		int* samples = new int[rendererData.textureCount];
		for (int i = 0; i < rendererData.textureCount; ++i) samples[i] = i;

		rendererData.spriteShader->Use();
		rendererData.spriteShader->SetIntArray("uTextures", samples, rendererData.textureCount);
		delete[] samples;

		for (int i = 0; i < rendererData.textureCount; ++i)
			rendererData.textures[i]->Bind(i);

		rendererData.spriteVertexArray->Bind();
		rendererData.spriteInstanceBuffer->Bind(0);
		rendererData.spriteInstanceBuffer->SetData(rendererData.spriteBeginInstances, rendererData.spriteIndex, 0);

		RendererApi::DrawTriangleInstanced(6, rendererData.spriteIndex, rendererData.indexBuffer->GetType());

		rendererData.textures[0]->UnBind();
	}

	void Renderer2D::FlushCircle() const
	{
		rendererData.circleShader->Use();
		rendererData.circleVertexArray->Bind();
		rendererData.circleInstanceBuffer->Bind(0);
		rendererData.circleInstanceBuffer->SetData(rendererData.circleBeginInstances, rendererData.circleIndex, 0);

		RendererApi::DrawTriangleInstanced(6, rendererData.circleIndex, rendererData.indexBuffer->GetType());
	}

	void Renderer2D::FlushLine() const
	{
		rendererData.lineShader->Use();
		rendererData.lineVertexArray->Bind();
		rendererData.lineInstanceBuffer->Bind(0);
		rendererData.lineInstanceBuffer->SetData(rendererData.lineBeginInstances, rendererData.lineIndex, 0);

		RendererApi::DrawLines(rendererData.lineIndex);
	}
	
	void Renderer2D::FlushText() const
	{
		if (rendererData.lastFont)
		{
			rendererData.textShader->Use();
			rendererData.textShader->SetInt("uTexture", 0);
			rendererData.lastFont->_texture->Bind();
			
			rendererData.textVertexArray->Bind();
			rendererData.textInstanceBuffer->Bind(0);
			rendererData.textInstanceBuffer->SetData(rendererData.textBeginInstances, rendererData.textIndex, 0);

			RendererApi::DrawTriangle(rendererData.textIndex);
		}
	}

	int Renderer2D::SetTextures(Ref<Texture> texture) const
	{
		if (texture == nullptr)
			return 0;

		int index = -1;
		for (int i = 0; i < rendererData.maxTextureUnits; ++i)
		{
			if (*rendererData.textures[i] == *texture)
			{
				index = i;
				break;
			}
			else if (rendererData.textures[i] == nullptr)
			{
				rendererData.textures[i] = texture;
				index = i;
				rendererData.textureCount++;
				break;
			}
		}

		if (rendererData.textureCount <= index)
			rendererData.textureCount = index + 1;

		if (index == -1)
		{
			Flush();
			Begin();

			rendererData.textures[1] = texture;
			rendererData.textureCount++;
		}
		return index;
	}
}