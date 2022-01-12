#version 450

layout (location = 0) out vec4 fragColor;

layout (location = 0) in vec2 uv;
layout (location = 1) in flat int textureIndex;
layout (location = 2) in flat uint packedColor;

#define MAX_TEXTURES 32

uniform sampler2D uTextures[MAX_TEXTURES];

void main()
{
	float r = ((packedColor >> 24) & 0xFF); r = r / 255.0;
	float g = ((packedColor >> 16) & 0xFF);  g = g / 255.0;
	float b = ((packedColor >> 8) & 0xFF); b = b / 255.0;
	float a = (packedColor & 0xFF); a = a / 255.0;

	fragColor = texture(uTextures[textureIndex], uv) * vec4(r, g, b, a);
}