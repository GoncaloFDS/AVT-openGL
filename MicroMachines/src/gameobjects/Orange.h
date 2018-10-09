#pragma once
#include "Model.h"

class Orange {
private:
	Model& m_Model;

	float m_Speed;
	float m_TurnSpeed;

	glm::vec3 m_Forward;
	glm::vec3 m_Right;
public:
	Orange(Model& model);
	~Orange();

	void OnUpdate();
};

