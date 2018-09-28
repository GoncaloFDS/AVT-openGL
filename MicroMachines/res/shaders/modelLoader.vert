#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 color;
uniform mat4 Matrix;

void main()
{
    TexCoords = aTexCoords;    
    color = vec4(aTexCoords, 1.0, 1.0);
    gl_Position = Matrix * vec4(aPos, 1.0);
}