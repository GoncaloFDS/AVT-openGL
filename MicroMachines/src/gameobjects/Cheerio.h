#pragma once
#include "SceneNode.h"

class Cheerio : public SceneNode {
private:
	float m_Speed;
	glm::vec3 m_Direction;

public:
	Cheerio();
	~Cheerio();

	virtual void OnCollision(SceneNode& other) override;

	virtual void OnUpdate(SceneNode& parent) override;

};

