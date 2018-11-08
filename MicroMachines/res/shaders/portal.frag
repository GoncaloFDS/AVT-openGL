#version 330

// textures of the object
uniform sampler2D rendered_texture;

in vec3 normalInterp;
in vec3 vertPos;
in vec2 texCoords;

out vec4 fragColor;

void main(){
	fragColor = texture(rendered_texture, texCoords);
}