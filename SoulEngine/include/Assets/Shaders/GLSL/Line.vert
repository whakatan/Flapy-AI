#version 450 core

out gl_PerVertex
{
	vec4 gl_Position;
};

struct Instance
{
	vec2 position;
	uint packedColor;
	float pad0;
};

layout (location = 0) out flat uint color;

layout (binding = 0, std140) uniform type_Camera
{
	mat4 projectionView;
} Camera;

layout (binding = 0, std140) readonly buffer type_Instance
{
	Instance m[];
} Instances;

void main()
{
	gl_Position = Camera.projectionView * vec4(Instances.m[gl_VertexID].position, 0.0, 1.0);
	color = Instances.m[gl_VertexID].packedColor;
}