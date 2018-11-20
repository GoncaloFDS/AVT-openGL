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
	GLCall(glEnable(GL_STENCIL_TEST));
	GLCall(glDepthFunc(GL_LEQUAL));
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glDepthRange(0.0, 1.0));
	GLCall(glClearDepth(1.0));
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));
	GLCall(glFrontFace(GL_CCW));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
	//GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));

	SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
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

void Renderer::SetStencilMask(GLuint mask) {
	GLCall(glStencilMask(mask));
}

void Renderer::SetStencilFunc(GLenum action, GLint ref, GLuint mask) {
	GLCall(glStencilFunc(action, ref, mask));
}

void Renderer::SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) {
	GLCall(glStencilOp(sfail, dpfail, dppass));
}

void Renderer::SetBlendFunc(GLenum sfactor, GLenum dfactor)
{
	GLCall(glBlendFunc(sfactor, dfactor));
}

void Renderer::ClearStencil() {
	GLCall(glClearStencil(0x00));
	GLCall(glClear(GL_STENCIL_BUFFER_BIT));
}

void Renderer::ClearDepth() {
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

void Renderer::DisableStencilTest() {
	GLCall(glDisable(GL_STENCIL_TEST));
}

void Renderer::EnableStencilTest() {
	GLCall(glEnable(GL_STENCIL_TEST));
}