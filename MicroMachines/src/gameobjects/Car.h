#pragma once
#include <glm/vec3.hpp> 
#include "SceneNode.h"


class Car : public SceneNode {
private:
	float m_Speed;
	float m_MaxSpeed;
	float m_Acceleration;
	float m_TurnSpeed;
	float m_Breaking;

	int m_HP;
	int m_MaxHP;
	SceneNode wheelRF, wheelLF, wheelLB, wheelRB;

public:
	Car();
	~Car();

	void SetWheelsModel(Model& wheel);
	void SetWheelsShader(Shader& shader);
	float GetSpeed();
	int GetHP();
	void LoseHP();
	void Stop();
	void Move(float amount);
	void Turn(float amount);
	void Reset();
	void Restart();
	void Teleport();
};

