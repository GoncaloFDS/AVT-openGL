#pragma once
#include <glm/vec3.hpp> 
#include "SceneNode.h"


class Car {
private:
	SceneNode m_Node;

	float m_Speed;
	float m_MaxSpeed;
	float m_Acceleration;
	float m_TurnSpeed;
	float m_Breaking;

	glm::vec3 m_Right;
public:
	Car();
	~Car();

	SceneNode& GetSceneNode();

	void Move(float amount);
	void Turn(float amount);
};

