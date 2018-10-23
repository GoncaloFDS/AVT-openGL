#version 330 core

layout(location = 0) in vec3 i_position;	
layout(location = 1) in vec3 i_normal;	
layout(location = 2) in vec2 i_texcoords;

uniform mat4 MVPMat;

out vec2 TexCoords;

void main(void) {
	TexCoords = i_texcoords;
	gl_Position = MVPMat * vec4(i_position, 1);
}