#include "Particle.h"
#include "cameras/Camera.h"

Particle::Particle(SceneNode *target)
{
	m_DefaultRotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
	m_Target = target;

	transform.scale = glm::vec3(0.1f);

	transform.position = glm::vec3(0.0f);
	m_Velocity = glm::vec3(0.0f);
	m_Color = glm::vec4(1.0f);
	m_LifeTime = 0.0f;
}


void Particle::Respawn() {
	float v_random = (rand() % 50 - 75) / 5.0f;
	float phi = glm::pi<float>() * (rand() % 100) / 100.0f;
	float theta = glm::pi<float>() * (rand() % 100) / 50.0f;
	float rColor = 0.5 + (rand() % 100) / 100.0f;
	transform.position = glm::vec3(0.0f);
	m_Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	m_LifeTime = .5f + rand() % (2 - 0 + 1) + 0;
	m_Velocity = glm::vec3(v_random * std::sin(phi) * std::cos(theta), v_random * std::cos(phi), v_random * std::sin(phi) * std::sin(theta));
	//m_Velocity = 0.1f * glm::vec3(v_random);
	m_Acceleration = glm::vec3(0.0f, -8.0f, 0.0f);
}

void Particle::OnUpdate(SceneNode &parent) {
	m_LifeTime -= Timer::deltaTime;

	if (IsAlive()) {
		m_Velocity += Timer::deltaTime * m_Acceleration;
		transform.position += Timer::deltaTime * m_Velocity;
		
		m_Color -= glm::vec4(0.0, 0.0, 0.0, Timer::deltaTime * 0.1f);
		LookAt(*m_Target);
	}

	SceneNode::OnUpdate(parent);
}

void Particle::OnRender(Camera &camera)
{
	if (m_LifeTime <= 0.0f) return;

	m_Shader->Bind();
	m_Shader->SetUniform4fv("color", m_Color);
	m_Shader->Unbind();

	SceneNode::OnRender(camera);
}

bool Particle::IsAlive() {
	return m_LifeTime > 0;
}

void Particle::SetTarget(SceneNode* target) {
	m_Target = target;
}
