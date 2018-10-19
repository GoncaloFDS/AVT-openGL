#include "SceneNode.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

SceneNode::SceneNode() {
	m_WorldMatrix = glm::mat4(1.0f);
}

SceneNode::~SceneNode() {
}

void SceneNode::OnUpdate(SceneNode& parent) {
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation) * glm::scale(glm::mat4(1.0f), transform.scale);
	m_WorldMatrix = parent.m_WorldMatrix * m_ModelMatrix;

	for (auto node : m_ChildNodes)
		node->OnUpdate(*this);
}

void SceneNode::OnRender(Camera& camera)
{
	if (m_Shader) {
		m_Shader->Bind();

		auto mvpMat = camera.GetViewProjMatrix() * m_WorldMatrix;
		m_Shader->SetUniformMat4f("MVPMat", mvpMat);
		
		m_Shader->SetUniformMat4f("ModelMat", m_WorldMatrix);

		auto normalMat = glm::transpose(glm::inverse(m_WorldMatrix));
		m_Shader->SetUniformMat3f("NormalMat", normalMat);

		m_Shader->SetUniform3fv("viewPos", camera.transform.position);

	}
	if(m_Model)
		m_Model->Draw(*m_Shader);
	
	for (auto& node : m_ChildNodes)
		node->OnRender(camera);
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