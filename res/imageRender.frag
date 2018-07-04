#version 330 core

in vec2 texCoords;

uniform sampler2D tex;

out vec4 color;

void main() {
	vec4 col = texture(tex, texCoords);
	color = vec4(col.xyz, 1.0f);
}