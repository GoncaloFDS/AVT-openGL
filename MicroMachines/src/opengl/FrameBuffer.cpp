#include "FrameBuffer.h"

#include "Common.h"

FrameBuffer::FrameBuffer() {
	GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::~FrameBuffer() {
	GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::Bind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::BindTexture(Texture texture) {
	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.id, 0));
	//GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.id, 0));
}

