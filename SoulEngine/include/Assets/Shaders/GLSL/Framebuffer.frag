#version 450

layout (location = 0) out vec4 fragColor;
layout (location = 0) in vec2 uv;

uniform sampler2D uTexture;

void main()
{
	fragColor = texture(uTexture, uv);
}