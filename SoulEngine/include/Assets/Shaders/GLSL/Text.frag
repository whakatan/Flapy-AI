#version 450
layout(location = 0) out vec4 fragColor;

layout (location = 0) in vec2 uv;
layout (location = 1) in vec4 color;

uniform sampler2D uTexture;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTexture, uv).r);
	fragColor = color * sampled;
}