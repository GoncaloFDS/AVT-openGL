#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "glm/vec4.hpp"

class Shader;

class Renderer {
public:
	Renderer();
	void Clear() const;
	void SetClearColor(glm::vec4 color) const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void SetStencilMask(GLuint mask);
	void SetStencilFunc(GLenum action, GLint ref, GLuint mask);
	void SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	void SetBlendFunc(GLenum sfactor, GLenum dfactor);
	void ClearStencil();
	void ClearDepth();
	void DisableStencilTest();
	void EnableStencilTest();
};

