#include "Portal.h"

#include "Common.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "RenderBuffer.h"
#include "Car.h"


Portal::Portal(glm::vec2 textureSize) {
	m_AABB = AABB(glm::vec3(-10, -3.5, -4), glm::vec3(10, 3.5, 4));

	m_Fb = new FrameBuffer();
	m_Texture = new Texture(glm::vec2(textureSize.x, textureSize.y));
	m_Rb = new RenderBuffer(glm::vec2(textureSize.x, textureSize.y));
	m_Fb->Bind();
	m_Fb->BindTexture(*m_Texture);
	m_Rb->AttachDepthRenderBuffer();
	m_Fb->Unbind();
}

void Portal::RenderToTexture(SceneGraph& sceneGraph, Renderer& renderer) {
	m_Fb->Bind();
	renderer.Clear();
	sceneGraph.OnRender();
	m_Fb->Unbind();
}

void Portal::BindTexture() {
	m_Shader->Bind();
	m_Texture->Bind();
	m_Shader->SetUniform1i("rendered_texture", 0);
}

void Portal::OnCollision(SceneNode& other) {
	auto& car = dynamic_cast<Car&>(other);
	car.Teleport();
}

