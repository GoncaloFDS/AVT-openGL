#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(glm::vec2 size) : m_Size(size) {
	GLCall(glGenRenderbuffers(1, &m_RendererID));
	//depth
	

	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));

}

RenderBuffer::~RenderBuffer() {
	GLCall(glDeleteRenderbuffers(1, &m_RendererID));
}

void RenderBuffer::Bind() {
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}

void RenderBuffer::Unbind() {
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

void RenderBuffer::AttachDepthRenderBuffer() {
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Size.x, m_Size.y));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RendererID));

	ASSERT((glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE));
}
