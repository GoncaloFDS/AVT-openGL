#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(SceneNode *target, Model &model, Shader &shader)
{
	lastUsedParticle = 0;

	particles.clear();

	for (unsigned int i = 0; i < n_particles; i++)
	{
		Particle *new_particle = new Particle(target);
		new_particle->SetModel(model);
		new_particle->SetShader(shader);
		particles.emplace_back(*new_particle);
	}

	for (auto &p : particles)
	{
		AddChildNode(&p);
	}
}

ParticleEmitter::~ParticleEmitter()
{
	particles.clear();
}

void ParticleEmitter::setCamera(SceneNode *camera)
{
	for (auto &p : particles)
	{
		p.setCamera(camera);
	}
}

void ParticleEmitter::OnUpdate(SceneNode &parent)
{
	for (unsigned int i = 0; i < n_new_particles; i++)
	{
		unsigned int unusedParticle = firstUnusedParticle();
		particles[unusedParticle].respawn();
	}

	SceneNode::OnUpdate(parent);
}

unsigned int ParticleEmitter::firstUnusedParticle()
{
	for (unsigned int i = lastUsedParticle; i < n_particles; i++)
	{
		if (particles[i].getLife() <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < n_particles; i++)
	{
		if (particles[i].getLife() <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	lastUsedParticle = 0;
	return 0;
}
