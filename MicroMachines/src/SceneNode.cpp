#include "SceneNode.h"
#include "Shader.h"
#include "Model.h"
#include "cameras/Camera.h"
#include "AABB.h"

SceneNode::SceneNode() {
	m_WorldMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::mat4(1.0f);
	m_Enabled = true;
}

SceneNode::~SceneNode() {
}

void SceneNode::OnUpdate(SceneNode& parent) {
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation) * glm::scale(glm::mat4(1.0f), transform.scale);
	m_WorldMatrix = parent.m_WorldMatrix * m_ModelMatrix;

	for (const auto& node : m_ChildNodes)
		node->OnUpdate(*this);

	m_AABB.OnUpdate(transform.position);
}

void SceneNode::OnRender(Camera& camera) {
	if (!m_Enabled)
		return;
	
	if (m_Shader) {
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("projection", camera.GetProjMatrix());

		//auto modelview = camera.GetViewMatrix() * m_WorldMatrix;
		m_Shader->SetUniformMat4f("view", camera.GetViewMatrix());
		m_Shader->SetUniformMat4f("model", m_WorldMatrix);
		m_Shader->SetUniform3fv("eyePos", camera.GetPosition());

		//auto mvpMat = camera.GetViewProjMatrix() * m_WorldMatrix;
		//m_Shader->SetUniformMat4f("MVPMat", mvpMat);
		//
		//m_Shader->SetUniformMat4f("ModelMat", m_WorldMatrix);

		auto normalMat = glm::transpose(glm::inverse(m_WorldMatrix));
		m_Shader->SetUniformMat4f("normalMat", normalMat);

		//m_Shader->SetUniform3fv("viewPos", camera.transform.position);

	}
	if (m_Model)
		m_Model->Draw(*m_Shader);

	for (auto& node : m_ChildNodes)
		node->OnRender(camera);
}

void SceneNode::OnCollision(SceneNode& other) {

}

void SceneNode::BindTextures(Shader& shader) {
	if (m_Model)
		m_Model->BindTextures(shader);
}

void SceneNode::SetShader(Shader& shader, bool propagate) {
	m_Shader = &shader;
	if (propagate) {
		for (auto child : m_ChildNodes)
			child->SetShader(shader, true);
	}
}

void SceneNode::SetModel(Model& model) {
	m_Model = &model;
}


void SceneNode::SetEnabled(bool b) {
	m_Enabled = b;
}


bool SceneNode::GetEnabled() {
	return m_Enabled;
}

AABB& SceneNode::GetAABB() {
	return m_AABB;
}


glm::vec3 SceneNode::GetForwardVector() {
	return m_Forward;
}


glm::vec3 SceneNode::GetRightVector() {
	return m_Right;
}


glm::vec3 SceneNode::GetUpVector() {
	return m_Up;
}

void SceneNode::AddChildNode(SceneNode* node) {
	m_ChildNodes.push_back(node);
}

CollisionData SceneNode::CheckCollision(SceneNode& other) {
	return m_AABB.CheckCollision(other.GetAABB());
}

void SceneNode::LookAt(SceneNode target) {
	glm::vec3 dir = target.transform.position - transform.position;
	dir.y = 0;
	dir = glm::normalize(dir);
	glm::quat rot = glm::quatLookAt(dir, glm::vec3(0, 1, 0));
	transform.rotation = rot * m_DefaultRotation;
}
