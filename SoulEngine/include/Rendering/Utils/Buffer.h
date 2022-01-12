#pragma once
#include <vector>

namespace SoulEngine
{
    enum class Buffers : int
    {
        Vertex,
        Index,
        Structured,
        Constant
    };

    enum class IndexType : int
    {
        Byte,
        UShort,
        UInt
    };

    enum class BindingFlags : int
    {
        Static,
        Dynamic
    };

    enum class BufferAccess : int
    {
        Read,
        Write,
        ReadWrite
    };

    enum class LayoutFormat
    {
        Float,
        Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        UInt, UInt2, UInt3, UInt4,
        Bool, Bool2, Bool3, Bool4,
        Mat3, Mat4
    };

    enum class FramebufferTextureFormat
    {
        None,
        RGB,
        RGBA8,
        RGB16F,
        RGB32F,
        RedInteger,
        Depth,
        Depth32F,
        Depth24Stencil8
    };

    struct FramebufferProperties
    {
        int width;
        int height;
        int samples = 1;
        FramebufferTextureFormat depthFormat;
        FramebufferTextureFormat textureFormat;
    };
}
