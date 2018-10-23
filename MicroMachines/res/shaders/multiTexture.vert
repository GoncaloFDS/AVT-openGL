#version 330

// attributes
layout(location = 0) in vec3 i_position;	// xyz - position
layout(location = 1) in vec3 i_normal;	// xyz - normal
layout(location = 2) in vec2 i_texcoords;	// xy - texture coords

// matrices
uniform mat4 MVPMat;
uniform mat4 ModelMat;
uniform mat3 NormalMat; // to transform normals, pre-perspective


// data for fragment shader
out vec3 FragPosition;
out vec3 Normal;
out vec2 TexCoords;

///////////////////////////////////////////////////////////////////

void main(void) {
	FragPosition = vec3(ModelMat * vec4(i_position, 1.0));
	Normal = NormalMat * i_normal;
	TexCoords = i_texcoords;
	gl_Position = MVPMat * vec4(i_position, 1.0);
}