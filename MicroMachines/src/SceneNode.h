#pragma once

#include <glm/mat4x4.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include <map>
#include "AABB.h"

class Model;
class Shader;
class Camera;

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
	AABB m_AABB;

public:
	SceneNode();
	~SceneNode();

	virtual void OnUpdate(SceneNode& parent);
	virtual void OnRender(Camera& camera);

	void SetShader(Shader& shader);
	void SetModel(Model& model);

	AABB& GetAABB();

	CollisionData CheckCollision(SceneNode& other);
	void AddChildNode(SceneNode* node);
};

