#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/constants.hpp>

#include "SceneNode.h"
#include "Shader.h"
#include "Timer.h"

class Particle :
	public SceneNode
{
private:

	static const float step;

	SceneNode *m_target;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec4 color;
	float life;

public:
	Particle(SceneNode *target);
	~Particle();
	void respawn();
	float getLife();
	void setCamera(SceneNode *target);
	virtual void OnUpdate(SceneNode &parent) override;

};

