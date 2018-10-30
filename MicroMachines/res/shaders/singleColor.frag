#version 330

struct LightProperties {
	bool isEnabled;
	bool isLocal;				//true for a point or spot light, false for directional lights
	bool isSpot;
	vec3 ambient;				//lights contribution to ambient light
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 direction;			//direction of highlights for directional light and spotlight
	float spotCutoff;
	float spotOutterCutOff;
	float constantAttenuation;	//local light attenuation coefficients
	float linearAttenuation;
	float quadraticAttenuation;
};

const int MaxLights = 10;
uniform LightProperties Lights[MaxLights];
uniform vec3 viewPos;

// textures of the object
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

uniform float Shininess = 12;

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main(){
	FragColor = vec4(0.1, 0.2, 0.8, 1.0);
}