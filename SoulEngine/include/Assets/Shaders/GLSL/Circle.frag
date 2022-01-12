#version 450

layout (location = 0) out vec4 fragColor;

layout (location = 0) in vec2 position;
layout (location = 1) in flat uint color;
layout (location = 2) in float thickness;

void main()
{
	float r = ((color >> 24) & 0xFF); r = r / 255.0;
	float g = ((color >> 16) & 0xFF);  g = g / 255.0;
	float b = ((color >> 8) & 0xFF); b = b / 255.0;
	float a = (color & 0xFF); a = a / 255.0;
	vec4 color = vec4(r, g, b, a);

	float dist = 1.0 - length(position);
	float alpha = smoothstep(0.0, 0.05f, dist);
	alpha *= smoothstep(thickness + 0.05f, thickness, dist);

	fragColor = color;
	fragColor.a *= alpha;
}