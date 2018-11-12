#pragma once

#define GLM_ENABLE_EXPERIMENTAL true

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/constants.hpp>
#include "SceneNode.h"
#include "shader.h"

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec4 color;
	float life;

	SceneNode node;
};

class ParticleEmitter : public SceneNode
{
	SceneNode *m_Target;
	std::vector<Particle> particles;
	const unsigned int n_particles = 10;
	const unsigned int n_new_particles = 5;
	const unsigned int step = 0.034;
	unsigned int lastUsedParticle;

	void respawnParticle(Particle &particle);

public:
	ParticleEmitter(SceneNode *target, Model &model, Shader &shader);
	~ParticleEmitter();

	virtual void OnUpdate(SceneNode& parent) override;
	unsigned int firstUnusedParticle();
};

