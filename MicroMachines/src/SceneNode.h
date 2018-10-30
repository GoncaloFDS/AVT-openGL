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
	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation = glm::quat(1, 0, 0, 0);
	glm::vec3 scale = glm::vec3(1.0f);
};


class SceneNode {
public:
	Transform transform;
	glm::mat4 m_WorldMatrix;
	glm::vec3 m_Forward;

protected:
	std::vector<SceneNode*> m_ChildNodes;
	bool m_Enabled;
	glm::mat4 m_ModelMatrix;
	Model* m_Model;
	Shader* m_Shader;
	AABB m_AABB;


public:
	SceneNode();
	~SceneNode();

	virtual void OnUpdate(SceneNode& parent);
	virtual void OnRender(Camera& camera);
	virtual void OnCollision(SceneNode& other);
	virtual void BindTextures(Shader& shader);
	CollisionData CheckCollision(SceneNode& other);

	void SetShader(Shader& shader);
	void SetModel(Model& model);
	void SetEnabled(bool b);
	bool GetEnabled();
	AABB& GetAABB();

	void AddChildNode(SceneNode* node);
};

