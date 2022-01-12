#version 450 core

out gl_PerVertex
{
	vec4 gl_Position;
};

struct Instance
{
	mat4 transform;
	uint packedColor;
	float thickness;
	vec2 pad0;
};

layout (location = 0) out vec2 position;
layout (location = 1) out flat uint color;
layout (location = 2) out float thickness;

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
	vec2 vertex = (vec2(float((uint(gl_VertexID) & 1u) << 1u), float(uint(gl_VertexID) & 2u)) - vec2(1.0));

	// transform
	gl_Position = Camera.projectionView * Instances.m[gl_InstanceID].transform * vec4(vertex * vec2(0.5), 0.0, 1.0);
   
	position = vertex;
	color = Instances.m[gl_InstanceID].packedColor;
	thickness = Instances.m[gl_InstanceID].thickness;
}