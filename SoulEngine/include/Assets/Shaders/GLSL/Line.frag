#version 450

layout (location = 0) out vec4 fragColor;

layout (location = 0) in flat uint color;

void main()
{
	float r = ((color >> 24) & 0xFF); r = r / 255.0;
	float g = ((color >> 16) & 0xFF);  g = g / 255.0;
	float b = ((color >> 8) & 0xFF); b = b / 255.0;
	float a = (color & 0xFF); a = a / 255.0;

	fragColor = vec4(r, g, b, a);
}