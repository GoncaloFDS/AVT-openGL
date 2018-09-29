#version 330 core

out vec4 outColor;

in vec2 TexCoords;
in vec3 FragPosition;

uniform vec3 viewPosition;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform vec3 lightPos;


void main() {   
    vec3 lightColor = vec3(.3, 0.0, 0.0);
    float shininess = 0.1; 

    vec3 ambient = texture(texture_diffuse1, TexCoords).rgb;

    //diffuse
    vec3 norm = texture(texture_normal1, TexCoords).rgb;
    vec3 lightDir = normalize(lightPos - FragPosition);
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = lightColor * (diff * texture(texture_diffuse1, TexCoords).rgb);

    //specular
    vec3 viewDir = normalize(viewPosition - FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = lightColor * (spec * texture(texture_specular1, TexCoords).rgb);

    vec3 result = ambient + diffuse + specular;

    outColor = vec4(result, 1.0);
}