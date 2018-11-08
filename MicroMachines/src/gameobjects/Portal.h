#pragma once

#include "Common.h"
#include "SceneNode.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "SceneGraph.h"
#include "Renderer.h"


class Portal : public SceneNode {
private:
	Portal* m_Destination;
	Texture* m_Texture;
	FrameBuffer* m_Fb;
	RenderBuffer* m_Rb;
	
public:
	Portal(glm::vec2 textureSize);
	~Portal() = default;

	void RenderToTexture(SceneGraph& sceneGraph, Renderer& renderer);
	void BindTexture();

	virtual void OnCollision(SceneNode& other) override;

};