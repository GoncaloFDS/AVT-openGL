#pragma once
#include "SceneNode.h"

class Cheerio : public SceneNode {
	float m_Speed;
	glm::vec3 direction;

public:
	Cheerio();
	~Cheerio();

	void OnCollision(SceneNode& other);

	virtual void OnUpdate(SceneNode& parent) override;

};

