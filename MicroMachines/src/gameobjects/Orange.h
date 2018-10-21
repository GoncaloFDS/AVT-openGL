#pragma once
#include "Model.h"

class Orange : public SceneNode{
private:

	float m_Speed;
	float m_TurnSpeed;

	glm::vec3 m_Right;
public:
	Orange();
	~Orange();

	void OnUpdate();
};

