#version 450 core

out gl_PerVertex
{
	vec4 gl_Position;
};

struct Instance
{
	mat4 transform;
	vec4 textureRect;
	int textureIndex;
	uint packedColor;
	vec2 pad0;
};

layout (location = 0) out vec2 uv;
layout (location = 1) out flat int textureIndex;
layout (location = 2) out flat uint packedColor;

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
	vec2 vertex = (vec2(float((uint(gl_VertexID) & 1u) << 1u), float(uint(gl_VertexID) & 2u)) - vec2(1.0)) * vec2(0.5);
	// transform
	gl_Position = Camera.projectionView * Instances.m[gl_InstanceID].transform * vec4(vertex, 0.0, 1.0);
   
	vec4 rect = Instances.m[gl_InstanceID].textureRect;
	textureIndex = Instances.m[gl_InstanceID].textureIndex;
	uv = vec2((vertex.x + 0.5) * rect.z + rect.x, (vertex.y + 0.5) * rect.w + rect.y);
	packedColor = Instances.m[gl_InstanceID].packedColor;
}