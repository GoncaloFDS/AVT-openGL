#pragma once

#include "SceneNode.h"
#include "Particle.h"

class ParticleEmitter : public SceneNode
{
	std::vector<Particle> particles;
	const unsigned int n_particles = 300;
	const unsigned int n_new_particles = 5;
	unsigned int lastUsedParticle;

public:
	ParticleEmitter(SceneNode *target, Model &model, Shader &shader);
	~ParticleEmitter();

	void setCamera(SceneNode *camera);
	void OnUpdate(SceneNode &parent) override;
	unsigned int firstUnusedParticle();
};

