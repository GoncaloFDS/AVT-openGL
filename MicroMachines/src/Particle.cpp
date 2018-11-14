#include "Particle.h"

const float Particle::step = 0.125;

Particle::Particle(SceneNode *target)
{
	m_DefaultRotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
	m_target = target;

	transform.scale = glm::vec3(0.1f);

	position = glm::vec3(0.0f);
	velocity = glm::vec3(0.0f);
	color = glm::vec4(1.0f);
	life = 0.0f;
}

Particle::~Particle()
{
}

void Particle::respawn()
{
	float v_random = (rand() % 50 - 75) / 5.0f;
	float phi = glm::pi<float>() * (rand() % 100) / 100.0f;
	float theta = glm::pi<float>() * (rand() % 100) / 50.0f;
	float rColor = 0.5 + (rand() % 100) / 100.0f;
	position = glm::vec3(0.0f);
	color = glm::vec4(rColor, rColor, rColor, 1.0f);
	life = 5.0f + (rand() % 8 - 2);
	velocity = glm::vec3(v_random * std::sin(phi) * std::cos(theta), v_random * std::cos(phi), v_random * std::sin(phi) * std::sin(theta));
	acceleration = glm::vec3(0.0f, -1.0f, 0.0f);
}

void Particle::OnUpdate(SceneNode &parent)
{
	life -= step;

	if (life > 0.0f)
	{
		position.x += step * velocity.x;
		position.y += step * velocity.y;
		position.z += step * velocity.z;
		velocity.x += step * acceleration.x;
		velocity.y += step * acceleration.y;
		velocity.z += step * acceleration.z;
		
		transform.position = position;
		color -= glm::vec4(step / 7);
	}

	LookAt(*m_target);
	SceneNode::OnUpdate(parent);
}

void Particle::OnRender(Camera &camera)
{
	if (life <= 0.0f) return;

	auto shader = GetShader();
	shader->Bind();
	shader->SetUniform4fv("color", color);
	shader->Unbind();

	SceneNode::OnRender(camera);
}

float Particle::getLife()
{
	return life;
}

void Particle::setCamera(SceneNode *target)
{
	m_target = target;
}
