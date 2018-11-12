#pragma once
#include "Texture.h"



class FrameBuffer {
private:
	unsigned int m_RendererID;

public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void Unbind();
	void BindTexture(Texture texture);
};