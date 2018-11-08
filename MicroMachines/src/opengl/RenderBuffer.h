#pragma once
#include "Common.h"
#include "FrameBuffer.h"

class RenderBuffer {
private:
	unsigned int m_RendererID;

	glm::vec2 m_Size;
public:
	RenderBuffer(glm::vec2 size);
	~RenderBuffer();

	void Bind();
	void Unbind();
	void AttachDepthRenderBuffer();

};