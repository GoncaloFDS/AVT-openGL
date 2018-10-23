#version 330 core

in vec2 TexCoords;
out vec4 out_Color;

uniform sampler2D texture_diffuse1;


void main(void) {
	out_Color =  texture(texture_diffuse1, TexCoords);
}