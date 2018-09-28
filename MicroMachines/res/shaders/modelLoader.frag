#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;


void main() {    
    FragColor = texture(texture_diffuse1 , TexCoords); 
}