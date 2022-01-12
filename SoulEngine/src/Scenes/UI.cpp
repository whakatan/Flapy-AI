#include "pch.h"
#include "Scenes/UI.h"
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
#include "Rendering/VertexBuffer.h"
#include "Assets/Fonts/FontManager.h"

namespace SoulEngine
{
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

	struct UIData
	{
		// circle
		int circleIndex{ 0 };
		int circleMaxInstance{ 0 };
		Ref<IndexBuffer> circleIndexBuffer{ nullptr };
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

		// rect
		int rectIndex{ 0 };
		int rectMaxInstance{ 0 };
		LineInstance* rectInstances{ nullptr };
		LineInstance* rectBeginInstances{ nullptr };
		Ref<Shader> rectShader{ nullptr };
		Ref<VertexArray> rectVertexArray{ nullptr };
		Ref<UniformBuffer> rectInstanceBuffer{ nullptr };

		// text
		int textIndex{ 0 };
		int textMaxInstance{ 0 };
		Ref<Font> lastFont{ nullptr };
		TextInstance* textInstances{ nullptr };
		TextInstance* textBeginInstances{ nullptr };
		Ref<Shader> textShader{ nullptr };
		Ref<VertexArray> textVertexArray{ nullptr };
		Ref<VertexBuffer> textVertexBuffer{ nullptr };
	};

	int index = 0;
	static UIData uiData;

	void UI::Init(int circleMaxInstance, int lineMaxInstance, int rectMaxInstance, int textMaxInstance)
	{
		byte* indices = new byte[6]{ 0, 1, 2, 2, 1, 3 };

		// circle
		uiData.circleIndex = 0;
		uiData.circleMaxInstance = circleMaxInstance;
		uiData.circleBeginInstances = new CircleInstance[circleMaxInstance];
		uiData.circleInstances = uiData.circleBeginInstances;
		uiData.circleShader = ShaderManager::Get("Circle");
		uiData.circleVertexArray = VertexArray::Create();
		uiData.circleInstanceBuffer = UniformBuffer::Create(Buffers::Structured, circleMaxInstance, sizeof(CircleInstance));
		uiData.circleIndexBuffer = IndexBuffer::Create(indices, sizeof(byte) * 6, IndexType::Byte);

		// line
		uiData.lineIndex = 0;
		uiData.lineMaxInstance = lineMaxInstance;
		uiData.lineBeginInstances = new LineInstance[lineMaxInstance];
		uiData.lineInstances = uiData.lineBeginInstances;
		uiData.lineShader = ShaderManager::Get("Line");
		uiData.lineVertexArray = VertexArray::Create();
		uiData.lineInstanceBuffer = UniformBuffer::Create(Buffers::Structured, lineMaxInstance, sizeof(LineInstance));

		// line
		uiData.rectIndex = 0;
		uiData.rectMaxInstance = rectMaxInstance;
		uiData.rectBeginInstances = new LineInstance[rectMaxInstance];
		uiData.rectInstances = uiData.rectBeginInstances;
		uiData.rectShader = ShaderManager::Get("Line");
		uiData.rectVertexArray = VertexArray::Create();
		uiData.rectInstanceBuffer = UniformBuffer::Create(Buffers::Structured, rectMaxInstance, sizeof(LineInstance));

		// text
		uiData.textIndex = 0;
		uiData.textMaxInstance = textMaxInstance;
		uiData.textBeginInstances = new TextInstance[textMaxInstance];
		uiData.textInstances = uiData.textBeginInstances;
		uiData.textShader = ShaderManager::Get("Text");
		uiData.textVertexArray = VertexArray::Create();
		uiData.textVertexBuffer = VertexBuffer::Create(textMaxInstance * sizeof(TextInstance), { { LayoutFormat::Float4 }, { LayoutFormat::Float4 } });
	}

	void UI::Destroy()
	{
		delete[] uiData.textBeginInstances;
		delete[] uiData.circleBeginInstances;
		delete[] uiData.lineBeginInstances;
		delete[] uiData.rectBeginInstances;
	}

	void UI::Begin()
	{
		BeginCircle();
		BeginLine();
		BeginText();
		BeginRect();
	}

	void UI::DrawCircle(const glm::mat4& transform, const Color& color, float thickness)
	{
		if (uiData.circleIndex >= uiData.circleMaxInstance)
		{
			FlushCircle();
			BeginCircle();
		}
		uiData.circleInstances->transform = transform;
		uiData.circleInstances->packedColor = color.Zipped();
		uiData.circleInstances->thickness = thickness;
		uiData.circleInstances++;
		uiData.circleIndex++;
	}

	void UI::DrawCircle(float x, float y, float radius, const Color& color, float thickness)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, 0.0f })* glm::scale(glm::mat4(1.0f), { radius * 2.0f, radius * 2.0f, 0.0f });
		DrawCircle(transform, color, thickness);
	}

	void UI::FillCircle(float x, float y, float radius, const Color& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, 0.0f }) * glm::scale(glm::mat4(1.0f), { radius * 2.0f, radius * 2.0f, 0.0f });
		DrawCircle(transform, color, 1.0f);
	}

	void UI::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const Color& color)
	{
		if (uiData.lineIndex >= uiData.lineMaxInstance)
		{
			FlushLine();
			BeginLine();
		}

		auto c = color.Zipped();
		uiData.lineInstances->position = p0;
		uiData.lineInstances->packedColor = c;
		uiData.lineInstances++;

		uiData.lineInstances->position = p1;
		uiData.lineInstances->packedColor = c;
		uiData.lineInstances++;

		uiData.lineIndex += 2;
	}

	void UI::DrawPolygon(const glm::vec2* points, size_t count, const Color& color)
	{
		for (size_t i = 1; i < count; ++i)
			DrawLine(points[i - 1], points[i], color);
	}

	void UI::DrawRect(const glm::mat4& transform, const Color& color)
	{
		static const glm::vec4 vertices[4] = { glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.0f, 1.0f) };

		DrawLine(transform * vertices[0], transform * vertices[1], color);
		DrawLine(transform * vertices[1], transform * vertices[3], color);
		DrawLine(transform * vertices[3], transform * vertices[2], color);
		DrawLine(transform * vertices[2], transform * vertices[0], color);
	}

	void UI::DrawRect(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		DrawRect({ position, size }, color);
	}

	void UI::DrawRect(const glm::vec4& rect, const Color& color)
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

	void UI::FillRect(const glm::mat4& transform, const Color& color)
	{
		static const glm::vec4 vertices[4] = { glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.0f, 1.0f) };

		FillRect(transform * vertices[0], transform * vertices[1], color);
		FillRect(transform * vertices[1], transform * vertices[3], color);
		FillRect(transform * vertices[3], transform * vertices[2], color);
		FillRect(transform * vertices[2], transform * vertices[0], color);
	}

	void UI::FillRect(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		FillRect({ position, size }, color);
	}

	void UI::FillRect(const glm::vec4& rect, const Color& color)
	{
		glm::vec2 leftTop{ rect.x - rect.z * 0.5f, rect.y - rect.w * 0.5f };
		glm::vec2 rightTop{ rect.x + rect.z * 0.5f, rect.y - rect.w * 0.5f };
		glm::vec2 leftBottom{ rect.x - rect.z * 0.5f, rect.y + rect.w * 0.5f };
		glm::vec2 rightBottom{ rect.x + rect.z * 0.5f, rect.y + rect.w * 0.5f };

		Fill(leftBottom, color); Fill(rightBottom, color);
		Fill(leftTop, color); Fill(rightBottom, color);
		Fill(rightTop, color); Fill(leftTop, color);
	}

	void UI::FillPolygon(const glm::vec2* points, size_t count, const Color& color)
	{
		for (size_t i = 1; i < count; ++i)
			Fill(points[i], color);
	}

	void UI::DrawString(const std::string& text, const glm::vec2& position, const glm::vec2& scale, Ref<Font> font, const Color& color)
	{
		if (font == nullptr)
			return;

		if (uiData.textIndex >= uiData.textMaxInstance || (uiData.lastFont && uiData.lastFont != font))
		{
			FlushText();
			BeginText();
		}
		uiData.lastFont = font;

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
			if (uiData.textIndex >= uiData.textMaxInstance)
			{
				FlushText();
				BeginText();
				uiData.lastFont = font;
			}

			if (c == '\n')
			{
				x = position.x;
				y += uiData.textBeginInstances->vertex.w * scale.y;
			}

			Character* character = font->characters[c];
			if (!character) continue;

			const glm::vec4& rect = character->TextureRect;

			float xpos = x + (character->Bearing.x * scale.x) * aspectX;
			float ypos = position.y - ((character->Size.y - character->Bearing.y) * scale.y) * aspectY - y;

			float w = character->Size.x * scale.x * aspectX;
			float h = character->Size.y * scale.y * aspectY;

			uiData.textInstances->vertex = { xpos, ypos, rect.x, rect.y + rect.w };
			uiData.textInstances->color = clr;
			uiData.textInstances++;

			uiData.textInstances->vertex = { xpos + w, ypos, rect.x + rect.z, rect.y + rect.w };
			uiData.textInstances->color = clr;
			uiData.textInstances++;

			uiData.textInstances->vertex = { xpos, ypos + h, rect.x, rect.y };
			uiData.textInstances->color = clr;
			uiData.textInstances++;

			uiData.textInstances->vertex = { xpos, ypos + h, rect.x, rect.y };
			uiData.textInstances->color = clr;
			uiData.textInstances++;

			uiData.textInstances->vertex = { xpos + w, ypos, rect.x + rect.z, rect.y + rect.w };
			uiData.textInstances->color = clr;
			uiData.textInstances++;

			uiData.textInstances->vertex = { xpos + w, ypos + h, rect.x + rect.z, rect.y };
			uiData.textInstances->color = clr;
			uiData.textInstances++;

			x += character->Advance.x * scale.x * aspectX;
			uiData.textIndex += 6;
		}
	}

	void UI::Flush()
	{
		if (uiData.circleIndex) FlushCircle();
		if (uiData.lineIndex) FlushLine();
		if (uiData.textIndex) FlushText();
		if (uiData.rectIndex) FlushRect();
	}

	void UI::Fill(const glm::vec2& point, const Color& color)
	{
		if (uiData.rectIndex >= uiData.rectMaxInstance)
		{
			FlushRect();
			BeginRect();
		}

		auto c = color.Zipped();
		uiData.rectInstances->position = point;
		uiData.rectInstances->packedColor = c;
		uiData.rectInstances++;
		uiData.rectIndex++;
	}

	void UI::BeginCircle()
	{
		uiData.circleIndex = 0;
		uiData.circleInstances = uiData.circleBeginInstances;
	}

	void UI::BeginLine()
	{
		uiData.lineIndex = 0;
		uiData.lineInstances = uiData.lineBeginInstances;
	}

	void UI::BeginRect()
	{
		uiData.rectIndex = 0;
		uiData.rectInstances = uiData.rectBeginInstances;
	}

	void UI::BeginText()
	{
		uiData.lastFont = nullptr;
		uiData.textIndex = 0;
		uiData.textInstances = uiData.textBeginInstances;
	}

	void UI::FlushCircle()
	{
		uiData.circleShader->Use();
		uiData.circleVertexArray->Bind();
		uiData.circleInstanceBuffer->Bind(0);
		uiData.circleInstanceBuffer->SetData(uiData.circleBeginInstances, uiData.circleIndex, 0);

		RendererApi::DrawTriangleInstanced(6, uiData.circleIndex, uiData.circleIndexBuffer->GetType());
	}

	void UI::FlushLine()
	{
		uiData.lineShader->Use();
		uiData.lineVertexArray->Bind();
		uiData.lineInstanceBuffer->Bind(0);
		uiData.lineInstanceBuffer->SetData(uiData.lineBeginInstances, uiData.lineIndex, 0);

		RendererApi::DrawLines(uiData.lineIndex);
	}

	void UI::FlushRect()
	{
		uiData.rectShader->Use();
		uiData.rectVertexArray->Bind();
		uiData.rectInstanceBuffer->Bind(0);
		uiData.rectInstanceBuffer->SetData(uiData.rectBeginInstances, uiData.rectIndex, 0);

		RendererApi::DrawTriangle(uiData.rectIndex);
	}

	void UI::FlushText()
	{
		if (uiData.lastFont)
		{
			uiData.textShader->Use();
			uiData.textShader->SetInt("uTexture", 0);
			uiData.lastFont->_texture->Bind();

			uint32_t dataSize = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(uiData.textInstances) - reinterpret_cast<uint8_t*>(uiData.textBeginInstances));

			uiData.textVertexArray->Bind();
			uiData.textVertexBuffer->SetData(uiData.textBeginInstances, dataSize);
			RendererApi::DrawTriangle(uiData.textIndex);
		}
	}
}