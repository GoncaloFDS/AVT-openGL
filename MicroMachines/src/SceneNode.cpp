#include "SceneNode.h"
#include "Shader.h"
#include "Model.h"

SceneNode::SceneNode() {
	m_WorldMatrix = glm::mat4(1.0f);
}

SceneNode::~SceneNode() {
}

void SceneNode::OnUpdate(SceneNode& parent) {
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation) * glm::scale(glm::mat4(1.0f), transform.scale);
	m_WorldMatrix = parent.m_WorldMatrix * m_ModelMatrix;
	//m_ModelMatrix = glm::translate(glm::mat4(1.0f), parent.position) * glm::mat4_cast(parent.rotation) * glm::scale(glm::mat4(1.0f), parent.scale) *
	//	glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation) * glm::scale(glm::mat4(1.0f), transform.scale);

	for (auto node : m_ChildNodes)
		node->OnUpdate(*this);
}

void SceneNode::OnRender() {
	if (m_Shader) {
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_modelMat", m_WorldMatrix);
	}
	if(m_Model)
		m_Model->Draw(*m_Shader);
	
	for (auto& node : m_ChildNodes)
		node->OnRender();
}

void SceneNode::SetShader(Shader& shader) {
	m_Shader = &shader;
}

void SceneNode::SetModel(Model& model) {
	m_Model = &model;
}

void SceneNode::AddChildNode(SceneNode* node) {
	m_ChildNodes.push_back(node);
}