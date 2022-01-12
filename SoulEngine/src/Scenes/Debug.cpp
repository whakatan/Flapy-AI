#include "pch.h"
#include "Scenes/Debug.h"
#include "Scenes/MainCamera.h"
#include "Scenes/SceneManager.h"
#include "Assets/Shaders/ShaderManager.h"
#include "Assets/Fonts/Font.h"
#include "Rendering/RendererApi.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/UniformBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Assets/Fonts/FontManager.h"

namespace SoulEngine
{
#ifdef _DEBUG
	struct CircleInstance
	{
		glm::mat4 transform;
		uint32_t packedColor;
		float thickness;
		float fade;
		float pad0;
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

	struct DebugData
	{
		Ref<IndexBuffer> indexBuffer{ nullptr };

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

	bool Debug::showCollider;
	static DebugData debugData;

#endif
	void Debug::Init(int circleMaxInstance, int lineMaxInstance, int textMaxInstance)
	{
#ifdef _DEBUG
		byte* indices = new byte[6]{ 0, 1, 2, 2, 1, 3 };

		// circle
		debugData.circleIndex = 0;
		debugData.circleMaxInstance = circleMaxInstance;
		debugData.circleBeginInstances = new CircleInstance[circleMaxInstance];
		debugData.circleInstances = debugData.circleBeginInstances;
		debugData.circleShader = ShaderManager::Get("Circle");
		debugData.circleVertexArray = VertexArray::Create();
		debugData.circleInstanceBuffer = UniformBuffer::Create(Buffers::Structured, circleMaxInstance, sizeof(CircleInstance));
		debugData.indexBuffer = IndexBuffer::Create(indices, sizeof(byte) * 6, IndexType::Byte);

		// line
		debugData.lineIndex = 0;
		debugData.lineMaxInstance = lineMaxInstance;
		debugData.lineBeginInstances = new LineInstance[lineMaxInstance];
		debugData.lineInstances = debugData.lineBeginInstances;
		debugData.lineShader = ShaderManager::Get("Line");
		debugData.lineVertexArray = VertexArray::Create();
		debugData.lineInstanceBuffer = UniformBuffer::Create(Buffers::Structured, lineMaxInstance, sizeof(LineInstance));

		// text
		debugData.textIndex = 0;
		debugData.textMaxInstance = textMaxInstance;
		debugData.textBeginInstances = new TextInstance[textMaxInstance];
		debugData.textInstances = debugData.textBeginInstances;
		debugData.textShader = ShaderManager::Get("Text");
		debugData.textVertexArray = VertexArray::Create();
		debugData.textInstanceBuffer = UniformBuffer::Create(Buffers::Structured, textMaxInstance, sizeof(TextInstance));
#endif
	}

	void Debug::Destroy()
	{
#ifdef _DEBUG
		delete[] debugData.textBeginInstances;
		delete[] debugData.circleBeginInstances;
		delete[] debugData.lineBeginInstances;
#endif

	}

	void Debug::Begin()
	{
#ifdef _DEBUG
		BeginCircle();
		BeginLine();
		BeginText();
#endif
	}

	void Debug::DrawCircle(const glm::mat4& transform, const Color& color, float thickness, float fade)
	{
#ifdef _DEBUG
		
		if (debugData.circleIndex >= debugData.circleMaxInstance)
		{
			FlushCircle();
			BeginCircle();
		}
		debugData.circleInstances->transform = transform;
		debugData.circleInstances->packedColor = color.Zipped();
		debugData.circleInstances->thickness = thickness;
		debugData.circleInstances->fade = fade;
		debugData.circleInstances++;
		debugData.circleIndex++;
#endif
	}

	void Debug::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const Color& color)
	{
#ifdef _DEBUG
		
		if (debugData.lineIndex >= debugData.lineMaxInstance)
		{
			FlushLine();
			BeginLine();
		}

		auto c = color.Zipped();
		debugData.lineInstances->position = p0;
		debugData.lineInstances->packedColor = c;
		debugData.lineInstances++;

		debugData.lineInstances->position = p1;
		debugData.lineInstances->packedColor = c;
		debugData.lineInstances++;

		debugData.lineIndex += 2;
#endif
	}

	void Debug::DrawRect(const glm::mat4& transform, const Color& color)
	{
#ifdef _DEBUG
		static const glm::vec4 vertices[4] = { glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.0f, 1.0f) };

		DrawLine(transform * vertices[0], transform * vertices[1], color);
		DrawLine(transform * vertices[1], transform * vertices[3], color);
		DrawLine(transform * vertices[3], transform * vertices[2], color);
		DrawLine(transform * vertices[2], transform * vertices[0], color);
#endif
	}

	void Debug::DrawRect(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
#ifdef _DEBUG
		DrawRect({ position, size }, color);
#endif
	}

	void Debug::DrawRect(const glm::vec4& rect, const Color& color)
	{
#ifdef _DEBUG
		
		glm::vec2 leftTop{ rect.x - rect.z * 0.5f, rect.y - rect.w * 0.5f };
		glm::vec2 rightTop{ rect.x + rect.z * 0.5f, rect.y - rect.w * 0.5f };
		glm::vec2 leftBottom{ rect.x - rect.z * 0.5f, rect.y + rect.w * 0.5f };
		glm::vec2 rightBottom{ rect.x + rect.z * 0.5f, rect.y + rect.w * 0.5f };

		DrawLine(leftBottom, rightBottom, color);
		DrawLine(rightBottom, rightTop, color);
		DrawLine(rightTop, leftTop, color);
		DrawLine(leftTop, leftBottom, color);
#endif
	}


	void Debug::DrawString(const std::string& text, const glm::vec2& position, const glm::vec2& scale, Ref<Font> font, const Color& color)
	{
#ifdef _DEBUG
		
		if (debugData.textIndex >= debugData.textMaxInstance || (debugData.lastFont && debugData.lastFont != font))
		{
			FlushText();
			BeginText();
		}
		debugData.lastFont = font;

		auto camera = SceneManager::Active()->GetCamera();

		float sizeH = camera->GetSize() * (1.0f / camera->GetZoom());
		float sizeW = camera->GetAspect() * sizeH;

		float aspectX = sizeW / RendererApi::GetWidth();
		float aspectY = sizeH / RendererApi::GetHeight();

		glm::vec4 clr = { color.R(), color.G(), color.B(), color.A() };

		float x = position.x;
		for (const char& c : text)
		{
			Character* character = font->characters[c];

			const glm::vec4& rect = character->TextureRect;

			float xpos = x + (character->Bearing.x * scale.x) * aspectX;
			float ypos = position.y - ((character->Size.y - character->Bearing.y) * scale.y) * aspectY;

			float w = character->Size.x * scale.x * aspectX;
			float h = character->Size.y * scale.y * aspectY;

			debugData.textInstances->vertex = { xpos, ypos, rect.x, rect.y + rect.w };
			debugData.textInstances->color = clr;
			debugData.textInstances++;

			debugData.textInstances->vertex = { xpos + w, ypos, rect.x + rect.z, rect.y + rect.w };
			debugData.textInstances->color = clr;
			debugData.textInstances++;

			debugData.textInstances->vertex = { xpos, ypos + h, rect.x, rect.y };
			debugData.textInstances->color = clr;
			debugData.textInstances++;

			debugData.textInstances->vertex = { xpos, ypos + h, rect.x, rect.y };
			debugData.textInstances->color = clr;
			debugData.textInstances++;

			debugData.textInstances->vertex = { xpos + w, ypos, rect.x + rect.z, rect.y + rect.w };
			debugData.textInstances->color = clr;
			debugData.textInstances++;

			debugData.textInstances->vertex = { xpos + w, ypos + h, rect.x + rect.z, rect.y };
			debugData.textInstances->color = clr;
			debugData.textInstances++;

			x += character->Advance.x * scale.x * aspectX;
			debugData.textIndex += 6;
		}
#endif
	}

	void Debug::Flush()
	{
#ifdef _DEBUG
		if (debugData.circleIndex) FlushCircle();
		if (debugData.lineIndex) FlushLine();
		if (debugData.textIndex) FlushText();
#endif
	}

	void Debug::BeginCircle()
	{
#ifdef _DEBUG
		debugData.circleIndex = 0;
		debugData.circleInstances = debugData.circleBeginInstances;
#endif
	}

	void Debug::BeginLine()
	{
#ifdef _DEBUG
		debugData.lineIndex = 0;
		debugData.lineInstances = debugData.lineBeginInstances;
#endif
	}

	void Debug::BeginText()
	{
#ifdef _DEBUG
		debugData.lastFont = nullptr;
		debugData.textIndex = 0;
		debugData.textInstances = debugData.textBeginInstances;
#endif
	}

	void Debug::FlushCircle()
	{
#ifdef _DEBUG
		debugData.circleShader->Use();
		debugData.circleVertexArray->Bind();
		debugData.circleInstanceBuffer->Bind(0);
		debugData.circleInstanceBuffer->SetData(debugData.circleBeginInstances, debugData.circleIndex, 0);

		RendererApi::DrawTriangleInstanced(6, debugData.circleIndex, debugData.indexBuffer->GetType());
#endif
	}

	void Debug::FlushLine()
	{
#ifdef _DEBUG
		debugData.lineShader->Use();
		debugData.lineVertexArray->Bind();
		debugData.lineInstanceBuffer->Bind(0);
		debugData.lineInstanceBuffer->SetData(debugData.lineBeginInstances, debugData.lineIndex, 0);

		RendererApi::DrawLines(debugData.lineIndex);
#endif
	}

	void Debug::FlushText()
	{
#ifdef _DEBUG
		if (debugData.lastFont)
		{
			debugData.textShader->Use();
			debugData.textShader->SetInt("uTexture", 0);
			debugData.lastFont->_texture->Bind();

			debugData.textVertexArray->Bind();
			debugData.textInstanceBuffer->Bind(0);
			debugData.textInstanceBuffer->SetData(debugData.textBeginInstances, debugData.textIndex, 0);

			RendererApi::DrawTriangle(debugData.textIndex);
		}
#endif
	}
}