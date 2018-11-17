#pragma once

#include "SceneNode.h"
#include "Particle.h"

class ParticleEmitter : public SceneNode {
	std::vector<Particle*> m_Particles;
	unsigned int m_ParticleCount = 200;
	unsigned int m_RespawnsPerFrame = 20;
	unsigned int m_LatestRespawn;

public:
	ParticleEmitter(SceneNode* target, Model& model, Shader& shader);
	~ParticleEmitter();

	Particle* GetADeadParticle();
	void SetTarget(SceneNode* target);

	void OnUpdate(SceneNode &parent) override;
};

