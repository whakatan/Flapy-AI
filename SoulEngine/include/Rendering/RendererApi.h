#pragma once
#include "Core.h"
#include "Utils/Buffer.h"

namespace SoulEngine
{
	class Scene;
	class UniformBuffer;
	class Renderer2D;
    struct CameraBuffer;

	class RendererApi
	{
	public:
        /// <summary>
        /// the maximum supported texture image
        /// </summary>
        /// <returns></returns>
        static int GetMaxTextureUnits();

        /// <summary>
        /// Render lines from array data.
        /// </summary>
        /// <param name="count"></param>
        static void DrawLines(int count);

        /// <summary>
        /// Render lines from array data.
        /// </summary>
        /// <param name="count"></param>
        static void DrawLines(int count, const IndexType& indexType);

        /// <summary>
        /// Render triangles from array data.
        /// </summary>
        /// <param name="count">Specifies the number of indices to be rendered.</param>
        static void DrawTriangle(int count);

        /// <summary>
        /// Render triangles from array data.
        /// </summary>
        /// <param name="count"></param>
        static void DrawTriangle(int count, const IndexType& indexType);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="count"></param>
        /// <param name="primcount"></param>
        static void DrawTriangleInstanced(int count, int primcount);

        /// <summary>
        /// 
        /// </summary>
        static void DrawTriangleInstanced(int count, int primcount, const IndexType& indexType, size_t indices = 0);

        static float GetLineWidth() { return _lineWidth; }
        static void SetLineWidth(float lineWidth);

        static int GetMultisample() { return _multisample; }
        static void SetMultisample(int multisample);

        static void Resize(int width, int height);
        static int GetWidth() { return _width; }
        static int GetHeight() { return _height; }

	private:
        static Ref<UniformBuffer> _cameraBuffer;
        static Unique<Renderer2D> _renderer2D;
        static float _lineWidth;
        static int _multisample;
        static int _width;
        static int _height;

        static bool _init;

        static void Init();
		static void DrawScene(Scene* scene);
#if _DEBUG
        static void DrawColliders(Scene* scene);
#endif
		static void Destroy();
        static void UniformCameraBuffer(const CameraBuffer& cameraBuffer);
        static uint32_t GetIndexTypeToOpenGL(const IndexType& type);

		friend class SceneManager;
	};
}