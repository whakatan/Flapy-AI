#include "pch.h"
#include "Rendering/RendererApi.h"
#include "Rendering/Framebuffer.h"
#include "Scenes/Scene.h"
#include "Scenes/MainCamera.h"
#include "Scenes/Renderer2D.h"
#include "Scenes/Debug.h"
#include "Assets/Shaders/ShaderManager.h"
#include "Assets/Textures/TextureManager.h"
#include "Rendering/UniformBuffer.h"
#include "Scenes/Entities/Behavior.h"
#include "Scenes/Entities/Components.h"
#include "Scenes/Entities/Colliders.h"
#include "Utils/Color.h"

#include <glad/gl.h>
#include <glfw/glfw3.h>

namespace SoulEngine
{
    bool RendererApi::_init;
    float RendererApi::_lineWidth { 1.0f };
    int RendererApi::_multisample{ 1 };
    int RendererApi::_width{ 100 };
    int RendererApi::_height{ 100 };
    Ref<UniformBuffer> RendererApi::_cameraBuffer;
    Unique<Renderer2D> RendererApi::_renderer2D;
    Ref<Framebuffer> _framebuffer;
    
    void GLAPIENTRY MessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
    {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
    }

    int RendererApi::GetMaxTextureUnits()
    {
        static int maxTextureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
        return maxTextureUnits;
    }

    void RendererApi::DrawLines(int count)
    {
        glDrawArrays(GL_LINES, 0, count);
    }

    void RendererApi::DrawLines(int count, const IndexType& indexType)
    {
        glDrawElements(GL_LINES, count, GetIndexTypeToOpenGL(indexType), nullptr);
    }

    void RendererApi::DrawTriangle(int count)
    {
        glDrawArrays(GL_TRIANGLES, 0, count);
    }

    void RendererApi::DrawTriangle(int count, const IndexType& indexType)
    {
        glDrawElements(GL_TRIANGLES, count, GetIndexTypeToOpenGL(indexType), nullptr);
    }

    void RendererApi::DrawTriangleInstanced(int count, int primcount)
    {
        glDrawArraysInstanced(GL_TRIANGLES, 0, count, primcount);
    }

    void RendererApi::DrawTriangleInstanced(int count, int primcount, const IndexType& indexType, size_t indices)
    {
        glDrawElementsInstanced(GL_TRIANGLES, count, GetIndexTypeToOpenGL(indexType), reinterpret_cast<const void*>(indices), primcount);
    }

    void RendererApi::Init()
    {
#ifdef _DEBUG
        //glEnable(GL_DEBUG_OUTPUT);
        //glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        //glDebugMessageCallback(MessageCallback, NULL);
        //
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
        ShaderManager::Load();
        TextureManager::Load();
        _renderer2D = CreateUnique<Renderer2D>(2048, 120, 120, 2048);

        _cameraBuffer = UniformBuffer::Create(Buffers::Constant, 1, sizeof(CameraBuffer));
        _framebuffer = Framebuffer::Create(ShaderManager::Get("Framebuffer"), { _width, _height, _multisample, FramebufferTextureFormat::None, FramebufferTextureFormat::RGB });

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        _init = true;
    }

    void RendererApi::DrawScene(Scene* scene)
	{
        _framebuffer->Bind();

        auto camera = scene->GetCamera();
        auto color = camera->clearColor;

        glClearColor(color.R(), color.G(), color.B(), color.A());
        glClear(GL_COLOR_BUFFER_BIT);

        CameraBuffer cameraBuffer{};
        camera->GetProjectionView(cameraBuffer.projectionView);

        UniformCameraBuffer(cameraBuffer);

        _renderer2D->Begin();
        {
            scene->_registry.group<SpriteRenderer>(entt::get<Transform>).each([&](auto& renderer, auto& transform) { if (renderer.visible) _renderer2D->DrawSprite(transform, renderer); });
            scene->_registry.group<CircleRenderer>(entt::get<Transform>).each([&](auto& renderer, auto& transform) { if (renderer.visible) _renderer2D->DrawCircle(transform, renderer); });
            scene->_registry.group<TextRenderer>(entt::get<Transform>).each([&](auto& renderer, auto& transform) { if (renderer.visible) _renderer2D->DrawString(transform, renderer); });
        }
        _renderer2D->Flush();

#if _DEBUG
        DrawColliders(scene);
#endif
        _framebuffer->Render();
	}

#if _DEBUG
    void RendererApi::DrawColliders(Scene* scene)
    {
        if (!Debug::showCollider) return;

        _renderer2D->Begin();
        constexpr glm::mat4 identity = glm::mat4(1.0f);

        {
            auto view = scene->_registry.view<BoxCollider2D, Transform>();
            for (auto entity : view)
            {
                auto [collider, transform] = view.get<BoxCollider2D, Transform>(entity);

                glm::vec3 translation = transform.position + glm::vec3(collider.GetOffset(), 0.001f);
                glm::vec3 scale = transform.scale * glm::vec3(collider.GetSize() * 2.0f, 1.0f);
                glm::mat4 translate = glm::translate(identity, translation) * glm::rotate(identity, glm::radians(transform.rotation.z), { 0.0f, 0.0f, 1.0f }) * glm::scale(identity, scale);

                _renderer2D->DrawRect(translate, Color::Green());
            }
        }

        {
            auto view = scene->_registry.view<CircleCollider2D, Transform>();
            for (auto entity : view)
            {
                auto [collider, transform] = view.get<CircleCollider2D, Transform>(entity);

                glm::vec3 translation = transform.position + glm::vec3(collider.GetOffset(), 0.001f);
                glm::vec3 scale = transform.scale * glm::vec3(collider.GetRadius() * 2.0f);
                glm::mat4 translate = glm::translate(identity, translation) * glm::scale(identity, scale);
                _renderer2D->DrawCircle(translate, Color::Green(), 1.0f);
            }
        }
        _renderer2D->Flush();
    }
#endif

    void RendererApi::Destroy()
    {
        _cameraBuffer.reset();
        _renderer2D.release();
    }

    void RendererApi::UniformCameraBuffer(const CameraBuffer& cameraBuffer)
    {
        _cameraBuffer->Bind(0);
        _cameraBuffer->SetData(&cameraBuffer);
    }

    uint32_t RendererApi::GetIndexTypeToOpenGL(const IndexType& type)
    {
        switch (type)
        {
        default:
        case IndexType::Byte: return GL_UNSIGNED_BYTE;
        case IndexType::UShort: return GL_UNSIGNED_SHORT;
        case IndexType::UInt: return GL_UNSIGNED_INT;
        }
    }
    
    void RendererApi::SetLineWidth(float lineWidth)
    {
        glLineWidth(lineWidth);
        _lineWidth = lineWidth;
    }

    void RendererApi::SetMultisample(int multisample)
    {
        _multisample = multisample;
        glfwWindowHint(GLFW_SAMPLES, multisample);

        if (!_init) return;
        _framebuffer->SetProperties({ _width, _height, _multisample, FramebufferTextureFormat::None, FramebufferTextureFormat::RGB });
    }

    void RendererApi::Resize(int width, int height)
    {
        _width = width; _height = height;

        if (!_init) return;
        _framebuffer->SetProperties({ _width, _height, _multisample, FramebufferTextureFormat::None, FramebufferTextureFormat::RGB });
    }
}