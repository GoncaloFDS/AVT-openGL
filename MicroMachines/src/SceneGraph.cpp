#include "SceneGraph.h"



SceneGraph::SceneGraph() {
	m_Root = new SceneNode();
}


SceneGraph::~SceneGraph() {
}

void SceneGraph::AddNode(SceneNode* node) {
	m_Root->AddChildNode(node);
}

SceneNode* SceneGraph::GetNode(const std::string& name) {
	return m_Root;
}

void SceneGraph::SetCamera(Camera& camera) {
	m_Camera = &camera;
}

Camera* SceneGraph::GetCamera() {
	return m_Camera;
}

void SceneGraph::OnUpdate() {
	m_Root->OnUpdate(*m_Root);
}

void SceneGraph::OnRender() {
	m_Root->OnRender();
}
