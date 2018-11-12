#version 330

// attributes
layout(location = 0) in vec3 i_position;	// xyz - position
layout(location = 1) in vec3 i_normal;	// xyz - normal
layout(location = 2) in vec2 i_texcoords;	// xy - texture coords

uniform mat4 projection, model, view;
uniform vec4 color;

// data for fragment shader
out vec2 TexCoords;
out vec4 ParticleColor;

void main(void) {
	TexCoords = i_texcoords;
	ParticleColor = color;
	gl_Position = projection * view * model * vec4(i_position, 1.0);
}