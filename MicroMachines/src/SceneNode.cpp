#include "SceneNode.h"

SceneNode::SceneNode(const std::string& name) 
	: m_Name(name) {

}

SceneNode::~SceneNode() {
}

void SceneNode::OnUpdate(Transform parentTransform) {
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), parentTransform.position) * glm::mat4_cast(parentTransform.rotation) * glm::scale(glm::mat4(1.0f), parentTransform.scale) *
		glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation) * glm::scale(glm::mat4(1.0f), transform.scale);

	for (auto node : m_ChildNodes)
		node->OnUpdate(transform);
}

void SceneNode::OnRender() {
	for (auto node : m_ChildNodes)
		node->OnRender();
	
	
}

void SceneNode::AddChildNode(SceneNode* node) {
	m_ChildNodes.push_back(node);
}

//void SceneNode::RegisterNode(std::map<const std::string&, SceneNode*>& nodeMap) {
//	nodeMap[m_Name] = this;
//
//	for (auto node : m_ChildNodes)
//		node->RegisterNode(nodeMap);
//}
