#version 330

// attributes
layout(location = 0) in vec3 i_position;	// xyz - position
layout(location = 1) in vec3 i_normal;	// xyz - normal
layout(location = 2) in vec2 i_texcoords;	// xy - texture coords

// matrices
uniform mat4 projection, model, view, normalMat;

out vec3 normalInterp;
out vec3 vertPos;
out vec2 texCoords;

///////////////////////////////////////////////////////////////////

void main(void) { 
	
	gl_Position = projection * view * model * vec4(i_position, 1.0);
	vec4 vertPos4 = model * vec4(i_position, 1.0);
	vertPos = vec3(vertPos4) /vertPos4.w;
	normalInterp = vec3(normalMat * vec4(i_normal, 0.0));
	texCoords = i_texcoords;
}