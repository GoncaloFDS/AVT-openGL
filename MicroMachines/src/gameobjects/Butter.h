#pragma once
#include "SceneNode.h"
class Butter : public SceneNode {
private:
	float m_Speed;
	glm::vec3 m_Direction;
public:
	Butter();
	~Butter();

	virtual void OnCollision(SceneNode& other) override;
	virtual void OnUpdate(SceneNode& parent) override;
};

