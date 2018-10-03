#pragma once

#include <vector>
#include <map>

#include "Camera.h"
#include "SceneNode.h"
#include "shader.h"

class SceneGraph {

private:
	Camera* m_Camera;
	SceneNode* m_Root;
	//std::map<const std::string&, SceneNode*> m_Nodes;

public:
	SceneGraph();
	~SceneGraph();

	void AddNode(SceneNode* node);

	SceneNode* GetNode(const std::string& name);
	void SetCamera(Camera& camera);
	Camera* GetCamera();

	void OnUpdate();
	void OnRender();
};

