#version 330

// textures of the object
uniform sampler2D texture_diffuse1;

in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 FragColor;

void main(){
	vec4 tex = (texture(texture_diffuse1, TexCoords)) * ParticleColor;
	if(tex.a == 0)
		discard;
	FragColor = tex;
}