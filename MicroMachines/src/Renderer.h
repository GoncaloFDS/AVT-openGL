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
};

