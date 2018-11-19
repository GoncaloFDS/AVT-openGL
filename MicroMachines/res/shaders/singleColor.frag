#version 330

struct LightProperties {
	bool isEnabled;
	bool isLocal;				//true for a point or spot light, false for directional lights
	bool isSpot;
	//vec3 ambient;				//lights contribution to ambient light
	vec3 diffuse;
	//vec3 specular;
	vec3 position;
	vec3 direction;			//direction of highlights for directional light and spotlight
	float spotCutoff;
	float spotOutterCutOff;
	float constantAttenuation;	//local light attenuation coefficients
	float linearAttenuation;
	float quadraticAttenuation;
};

const vec3 fogColor = vec3(0.5, 0.5, 0.5);
const int MaxLights = 10;
uniform LightProperties Lights[MaxLights];

// textures of the object
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

uniform float shininess = 128;
uniform vec3 eyePos;
uniform bool fogIsEnabled = true;

in vec3 normalInterp;
in vec3 vertPos;
in vec2 texCoords;

out vec4 fragColor;

void main(){
	fragColor = texture(texture_diffuse1, texCoords);
}