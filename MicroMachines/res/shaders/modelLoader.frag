#version 330

// data from vertex shader
in vec3	o_toLight;
in vec3	o_toCamera;
in vec2	o_texcoords;

// textures of the object
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

// color for framebuffer
out vec4 resultingColor;

/////////////////////////////////////////////////////////

// parameters of the light and possible values
uniform vec3 u_lightColor = vec3(1.0, 1.0, 1.0);

// parameters of the material and possible values
uniform float u_matShininess = 64;

/////////////////////////////////////////////////////////

// returns intensity of reflected ambient lighting


void main(void)
{
	//ambient
	vec3 ambient = u_lightColor * 0.;

	//diffuse
	vec3 normal = texture(texture_normal1, o_texcoords).rgb;
	float diff = max(dot(normal, o_toLight), 0.0);
	vec3 diffuse = diff * u_lightColor * texture(texture_diffuse1, o_texcoords).rgb;

	//specular
	vec3 reflectDir = reflect(-o_toLight, normal);
	float spec = pow(max(dot(o_toCamera, reflectDir), 0.0), u_matShininess);
	//vec3 specular = u_lightColor * spec;
	vec3 specular = texture(texture_specular1, o_texcoords).rgb * 0.1;


	vec3 result = ambient + diffuse + specular;
	resultingColor = vec4(result, 1.0);
}
