#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(SceneNode *target, Model &model, Shader &shader)
{
	m_LatestRespawn = 0;

	for (unsigned int i = 0; i < m_ParticleCount; i++) {
		Particle *new_particle = new Particle(target);
		new_particle->SetModel(model);
		new_particle->SetShader(shader);
		m_Particles.emplace_back(new_particle);
	}

	for (auto p : m_Particles) {
		AddChildNode(p);
	}
}

ParticleEmitter::~ParticleEmitter() {
	m_Particles.clear();
}

void ParticleEmitter::OnUpdate(SceneNode &parent) {
	for (unsigned int i = 0; i < m_RespawnsPerFrame; i++) {
		GetADeadParticle()->Respawn();
	}

	SceneNode::OnUpdate(parent);
}



Particle* ParticleEmitter::GetADeadParticle() {
	
	for (unsigned int i = m_LatestRespawn; i < m_ParticleCount; i++) {
		if (!m_Particles[i]->IsAlive())	{
			m_LatestRespawn = i;
			return m_Particles[i];
		}
	}

	for (unsigned int i = 0; i < m_ParticleCount; i++) {
		if (!m_Particles[i]->IsAlive()) {
			m_LatestRespawn = i;
			return m_Particles[i];
		}
	}

	m_LatestRespawn = 0;
	return m_Particles[0];
}

void ParticleEmitter::SetTarget(SceneNode* target) {
	for (auto particle : m_Particles)
		particle->SetTarget(target);
}
