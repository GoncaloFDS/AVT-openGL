#pragma once

#include <glm/mat4x4.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include <map>


struct Transform {
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale = glm::vec3(1.0f);
};


class SceneNode {
protected:
	std::string m_Name;
	std::vector<SceneNode*> m_ChildNodes;
	Transform transform;
	glm::mat4 m_ModelMatrix;

public:
	SceneNode(const std::string& name);
	~SceneNode();

	virtual void OnUpdate(glm::mat4 parentMatrix);
	virtual void OnRender();

	void AddChildNode(SceneNode* node);
	//void RegisterNode(std::map<const std::string&, SceneNode*>& nodeMap);
};

