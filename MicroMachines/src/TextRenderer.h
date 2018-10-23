#pragma once

// Code from https://learnopengl.com/In-Practice/2D-Game/Render-text
//TODO refactor class to use abstraction classes such as VertexArray
#include <map>

#include<glm/glm.hpp>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "GL/glew.h"

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;     // Horizontal offset to advance to next glyph
};

class TextRenderer {
public:
	// Holds a list of pre-compiled Characters
	std::map<GLchar, Character> Characters;
	// Shader used for text rendering
	Shader* TextShader;
	// Constructor
	TextRenderer(GLuint width, GLuint height);
	// Pre-compiles a list of characters from the given font
	void Load(std::string font, GLuint fontSize);
	// Renders a string of text using the precompiled list of characters
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
	void Resize(GLuint width, GLuint height);
private:
	// Render state
	GLuint VAO, VBO;
};

