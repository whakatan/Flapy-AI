#version 450

out gl_PerVertex
{
	vec4 gl_Position;
};

layout (location = 0) out vec2 uv;
layout (location = 1) out vec4 color;

layout (location = 0) in vec4 aVertex;
layout (location = 1) in vec4 aColor;

layout (binding = 0, std140) uniform type_Camera
{
	mat4 projectionView;
} Camera;

void main()
{
	gl_Position = Camera.projectionView * vec4(aVertex.xy, 0.0, 1.0f);
	uv = aVertex.zw;

	color = aColor;
}