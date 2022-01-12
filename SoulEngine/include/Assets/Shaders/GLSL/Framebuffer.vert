#version 450

out gl_PerVertex
{
	vec4 gl_Position;
};

layout (location = 0) out vec2 uv;

void main()
{
	vec2 vertex = vec2(float((uint(gl_VertexID) & 1u) << 1), float((uint(gl_VertexID) & 2u))) - vec2(1.0);
	gl_Position = vec4(vertex, 0.0, 1.0);
	uv = vec2(max(vertex.x, 0), max(vertex.y, 0.0));
}