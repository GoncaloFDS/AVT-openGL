#pragma once

#include <glm/mat4x4.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include <map>

class Model;
class Shader;

struct Transform {
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale = glm::vec3(1.0f);
};


class SceneNode {
public:
	Transform transform;
	glm::mat4 m_WorldMatrix;
	glm::vec3 m_Forward;
protected:
	std::vector<SceneNode*> m_ChildNodes;
	glm::mat4 m_ModelMatrix;
	Model* m_Model;
	Shader* m_Shader;

public:
	SceneNode();
	~SceneNode();

	virtual void OnUpdate(SceneNode& parent);
	virtual void OnRender();

	void SetShader(Shader& shader);
	void SetModel(Model& model);
	void AddChildNode(SceneNode* node);
};

