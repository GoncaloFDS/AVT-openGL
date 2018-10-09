#pragma once
#include "Model.h"

class Car {
private:
	Model& m_Model;

	float m_Speed;
	float m_MaxSpeed;
	float m_Acceleration;
	float m_TurnSpeed;

	glm::vec3 m_Forward;
	glm::vec3 m_Right;
public:
	Car(Model& model);
	~Car();

	void Move(float amount);
	void Turn(float amount);
};

