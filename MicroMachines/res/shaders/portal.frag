#version 330

// textures of the object
uniform sampler2D rendered_texture;

in vec3 normalInterp;
in vec3 vertPos;
in vec2 texCoords;

const vec3 fogColor = vec3(0.5, 0.5, 0.5);

uniform vec3 eyePos;
uniform bool fogIsEnabled = true;

out vec4 fragColor;

void main(){
	//Fog
	float distanceEF;
	float scattering = 1;
	float extinction = 1;
	if(fogIsEnabled) {
		distanceEF = length(vertPos - eyePos);
		scattering = exp(-distanceEF * 0.004);
		extinction = exp(-distanceEF * 0.01);
	}

	vec3 resultingColor = texture(rendered_texture, texCoords).xyz;
	fragColor = vec4(resultingColor * extinction + fogColor * (1 - scattering), 1);
}