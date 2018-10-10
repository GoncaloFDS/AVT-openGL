#pragma once
#include "Model.h"

class Orange {
private:
	SceneNode m_Node;

	float m_Speed;
	float m_TurnSpeed;

	glm::vec3 m_Right;
public:
	Orange();
	~Orange();

	SceneNode& GetSceneNode();

	void OnUpdate();
};

