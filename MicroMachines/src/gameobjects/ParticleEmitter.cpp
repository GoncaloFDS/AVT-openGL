#include "ParticleEmitter.h"

void ParticleEmitter::respawnParticle(Particle &particle)
{
	float random = (rand() % 100 - 50) / 10.0f;
	float v_random = (rand() % 50 - 25) / 5.0f;
	float phi = glm::pi<float>() * (rand() % 100) / 100.0f;
	float theta = glm::pi<float>() * (rand() % 100) / 50.0f;
	float rColor = 0.5 + (rand() % 100) / 100.0f;
	particle.position = glm::vec3(random);
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.life = 1.0f;
	particle.velocity = glm::vec3(v_random * std::sin(phi) * std::cos(theta), v_random * std::cos(phi), v_random * std::sin(phi) * std::sin(theta));
	particle.acceleration = glm::vec3(0.1f, -9.8f, 0.1f);
}

ParticleEmitter::ParticleEmitter(SceneNode *target, Model &model, Shader &shader) : m_Target(target)
{
	m_DefaultRotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));

	particles.clear();

	for (unsigned int i = 0; i < n_particles; i++)
	{
		Particle new_particle;
		new_particle.position = glm::vec3(0.0f);
		new_particle.velocity = glm::vec3(0.0f);
		new_particle.color = glm::vec4(1.0f);
		new_particle.life = 0.0f;
		new_particle.node.SetModel(model);
		new_particle.node.SetShader(shader);
		AddChildNode(&(new_particle.node));
		particles.emplace_back(new_particle);
	}
}

ParticleEmitter::~ParticleEmitter()
{
	particles.clear();
}

void ParticleEmitter::OnUpdate(SceneNode& parent)
{
	LookAt(*m_Target);

	for (unsigned int i = 0; i < n_new_particles; i++)
	{
		unsigned int unusedParticle = firstUnusedParticle();
		respawnParticle(particles[unusedParticle]);
	}

	for (auto &p : particles)
	{
		p.life -= step;

		if (p.life > 0.0f)
		{
			p.velocity -= p.acceleration;
			p.position += p.velocity * glm::diagonal3x3(glm::vec3(step));
			p.color -= step * 2.5;

			p.node.transform.position = p.position;
		}
	}
}

unsigned int ParticleEmitter::firstUnusedParticle()
{
	for (unsigned int i = lastUsedParticle; i < n_particles; i++)
	{
		if (particles[i].life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < n_particles; i++)
	{
		if (particles[i].life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	lastUsedParticle = 0;
	return 0;
}
