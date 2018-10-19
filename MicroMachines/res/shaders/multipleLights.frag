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

uniform float Shininess = 32;

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main(){
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPosition);

	vec3 result;

	for(int light = 0; light < MaxLights; light++){
		if(!Lights[light].isEnabled)
			continue;

		if(Lights[light].isLocal) {

			if(Lights[light].isSpot){
				vec3 lightDir = normalize(Lights[light].position - FragPosition);
				// diffuse shading
				float diff = max(dot(norm, lightDir), 0.0);
				// specular shading
				vec3 reflectedDir = reflect(-lightDir, norm);
				float spec = pow(max(dot(viewDir, reflectedDir), 0.0), Shininess);
				// attenuation
				float dist = length(Lights[light].position - FragPosition);
				float attenuation = 1.0 / 
					(Lights[light].constantAttenuation + 
					Lights[light].linearAttenuation * dist + 
					Lights[light].quadraticAttenuation * (dist * dist));   
				// spotlight intensity
				float theta = dot(lightDir, normalize(-Lights[light].direction)); 
				float epsilon = Lights[light].spotCutoff - Lights[light].spotOutterCutOff;
				float intensity = clamp((theta - Lights[light].spotOutterCutOff) / epsilon, 0.0, 1.0);
				// combine results
				vec3 ambient = Lights[light].ambient * vec3(texture(texture_diffuse1, TexCoords)) * attenuation * intensity;
				vec3 diffuse = Lights[light].diffuse * diff * vec3(texture(texture_diffuse1, TexCoords)) * attenuation * intensity;
				vec3 specular = Lights[light].specular * spec * vec3(texture(texture_specular1, TexCoords)) * attenuation * intensity;
				result += ambient + diffuse + specular;
			}
			else { 
				// point
				vec3 lightDir = normalize(Lights[light].position - FragPosition);
				//diffuse shading
				float diff = max(dot(norm, lightDir), 0.0);
				//specular shading
				vec3 reflectedDir = reflect(-lightDir, norm);
				float spec = pow(max(dot(viewDir, reflectedDir), 0.0), Shininess);
				// attenuation
				float dist = length(Lights[light].position - FragPosition);
				float attenuation = 1.0 / 
					(Lights[light].constantAttenuation + 
					Lights[light].linearAttenuation * dist + 
					Lights[light].quadraticAttenuation * (dist * dist));
				//combine results
				vec3 ambient = Lights[light].ambient * vec3(texture(texture_diffuse1, TexCoords)) * attenuation;
				vec3 diffuse = Lights[light].diffuse * diff * vec3(texture(texture_diffuse1, TexCoords)) * attenuation;
				vec3 specular = Lights[light].specular * spec * vec3(texture(texture_specular1, TexCoords)) * attenuation;
				result += ambient + diffuse + specular;
			}
		}
		else { 
			// directional
			vec3 lightDir = normalize(-Lights[light].direction);
			//diffuse shading
			float diff = max(dot(norm, lightDir), 0.0); 
			//speculat shading
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
			//combine results
			vec3 ambient = Lights[light].ambient * vec3(texture(texture_diffuse1, TexCoords));
			vec3 diffuse = Lights[light].diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
			vec3 specular = Lights[light].specular * spec * vec3(texture(texture_specular1, TexCoords));

			result += ambient + diffuse + specular;
		}

		
	}
	
	FragColor = vec4(result, 1.0);
}