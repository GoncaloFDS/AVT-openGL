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
	vec3 normal = normalize(normalInterp);
	//vec3 normal = texture(texture_normal1, texCoords).xyz;
	vec4 diffuseColor = texture(texture_diffuse1, texCoords);
	vec3 ambientColor = diffuseColor.xyz * 0.01;
	vec3 specColor = vec3(1.0, 1.0, 1.0);
	vec3 lightDir;
	float distance = 1;
	float distance2 = 1;
	vec3 resultingColor = vec3(0.0, 0.0, 0.0);

	if(diffuseColor.a == 0)
		discard;

	//Fog
	float distanceEF;
	float scattering = 1;
	float extinction = 1;
	if(fogIsEnabled) {
		distanceEF = length(vertPos - eyePos);
		scattering = exp(-distanceEF * 0.004);
		extinction = exp(-distanceEF * 0.01);
	}
	for(int i = 0; i < MaxLights; i++){
		if(!Lights[i].isEnabled)
			continue;

		if(Lights[i].isLocal){
			//vec3 eyespaceLightPos =  (mvMat * vec4(Lights[i].position, 1)).xyz;
			lightDir = Lights[i].position - vertPos;
			distance = length(lightDir);
			distance2 = distance * distance;
			lightDir = normalize(lightDir);
		}
		else
			lightDir = Lights[i].direction;

		
		float diffuse = max(dot(lightDir, normal), 0.0);
		float specular = 0.0;

		if(diffuse > 0.0) {

			vec3 viewDir = normalize(eyePos - vertPos);
			//blinn phong
			vec3 halfDir = normalize(lightDir + viewDir);
			float specAngle = max(dot(normal, halfDir), 0.0);
			specular = pow(specAngle, shininess);
		}
		float attenuation = 1.0;

		if(Lights[i].isLocal)
			attenuation = 1.0 / 
					(Lights[i].constantAttenuation + 
					Lights[i].linearAttenuation * distance + 
					Lights[i].quadraticAttenuation * distance2);

		if(Lights[i].isSpot){
			float theta = dot(lightDir, normalize(-Lights[i].direction));
			float epsilon = Lights[i].spotCutoff - Lights[i].spotOutterCutOff;
			float intensity = clamp((theta - Lights[i].spotOutterCutOff) / epsilon, 0.0, 1.0);
			diffuse *= intensity;
			specular *= intensity;
			
		}
					
		vec3 colorLinear = ambientColor + 
			diffuseColor.xyz * diffuse * Lights[i].diffuse * attenuation +
			specColor * specular * Lights[i].diffuse * attenuation * 0.2;

		resultingColor += colorLinear;
	}

	resultingColor = resultingColor * extinction + fogColor * (1 - scattering); 
	
	fragColor = vec4(resultingColor, texture(texture_diffuse1, texCoords).a);

////////////////////
}