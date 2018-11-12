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

protected:
	std::vector<SceneNode*> m_ChildNodes;
	bool m_Enabled;
	glm::mat4 m_ModelMatrix;
	Model* m_Model;
	Shader* m_Shader;
	AABB m_AABB;

	glm::vec3 m_Forward;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_WorldUp;
	glm::vec3 m_WorldPosition;

	glm::quat m_DefaultRotation = glm::quat(1, 0, 0, 0);

public:
	SceneNode();
	~SceneNode();

	virtual void OnUpdate(SceneNode& parent);
	virtual void OnRender(Camera& camera);
	virtual void OnCollision(SceneNode& other);
	virtual void BindTextures(Shader& shader);
	CollisionData CheckCollision(SceneNode& other);
	void LookAt(SceneNode target);

	void SetShader(Shader& shader, bool propagate = false);
	void SetModel(Model& model);
	void SetEnabled(bool b);
	bool GetEnabled();
	AABB& GetAABB();
	Shader* GetShader();


	glm::vec3 GetForwardVector(); 
	glm::vec3 GetRightVector();
	glm::vec3 GetUpVector();

	void AddChildNode(SceneNode* node);
};

