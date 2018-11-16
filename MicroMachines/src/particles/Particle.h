#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/constants.hpp>

#include "SceneNode.h"
#include "Shader.h"
#include "Timer.h"

class Particle : public SceneNode {
private:


	SceneNode* m_Target;
	
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;
	glm::vec4 m_Color;
	float m_LifeTime;

public:
	Particle(SceneNode *target);
	~Particle() = default;
	
	void Respawn();
	bool IsAlive();
	void SetTarget(SceneNode* target);

	virtual void OnUpdate(SceneNode &parent) override;
	virtual void OnRender(Camera &camera);
	
};

