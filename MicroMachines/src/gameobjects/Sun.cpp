#include "Sun.h"

const float Sun::sun_dist = 300.0f;

Sun::Sun(float scale, glm::vec3 direction, SceneNode* target)
{
	light = new DirectionalLight(direction);
	m_Target = target;
	m_DefaultRotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
}

void Sun::OnUpdate(SceneNode& parent)
{
	LookAt(*m_Target);
	SceneNode::OnUpdate(parent);
}

void Sun::SetTarget(SceneNode* target)
{
	m_Target = target;
}

glm::vec3 Sun::GetWorldPosition(glm::vec3 camPos)
{
	return sun_dist * light->getDirection() + camPos;
}

DirectionalLight* Sun::GetLight()
{
	return light;
}

void Sun::ToogleLight()
{
	light->ToogleLight();
}