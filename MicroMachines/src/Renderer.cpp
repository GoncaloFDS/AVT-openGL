#include "Renderer.h"

#include <iostream>

#include "Common.h"
#include "Shader.h"

Renderer::Renderer() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LEQUAL));
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glDepthRange(0.0, 1.0));
	GLCall(glClearDepth(1.0));
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));
	GLCall(glFrontFace(GL_CCW));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::SetClearColor(glm::vec4 color) const {
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}