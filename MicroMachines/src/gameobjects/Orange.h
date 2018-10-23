#pragma once
#include "Model.h"

class Orange : public SceneNode {
private:

	float m_Speed;
	float m_TurnSpeed;
	float turningAngle;
	glm::vec3 m_Right;
public:
	Orange();
	~Orange();

	virtual void OnUpdate(SceneNode& parent) override;
	virtual void OnCollision(SceneNode& other) override;

};

