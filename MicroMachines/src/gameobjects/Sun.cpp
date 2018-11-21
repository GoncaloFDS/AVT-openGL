#include "Sun.h"
#include "cameras/Camera.h"

const float Sun::sun_dist = 300.0f;

Sun::Sun(float scale, glm::vec3 direction, SceneNode* target)
{
	light = new DirectionalLight(direction);
	m_Target = target;
	m_DefaultRotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
	m_ShadowMatrix = glm::mat4(1);
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

glm::mat4 Sun::GetShadowMatrix()
{	
	glm::vec4 plane(0, 1, 0, 0);
	glm::vec3 lightPos(0, 20, 0);
	float dot = glm::dot(glm::vec3(plane), lightPos);

	m_ShadowMatrix[0][0] = dot - lightPos[0] * plane[0];
	m_ShadowMatrix[1][0] = -lightPos[0] * plane[1];
	m_ShadowMatrix[2][0] = -lightPos[0] * plane[2];
	m_ShadowMatrix[3][0] = -lightPos[0] * plane[3];
	m_ShadowMatrix[0][1] = -lightPos[1] * plane[0];
	m_ShadowMatrix[1][1] = dot - lightPos[1] * plane[1];
	m_ShadowMatrix[2][1] = -lightPos[1] * plane[2];
	m_ShadowMatrix[3][1] = -lightPos[1] * plane[3];
	m_ShadowMatrix[0][2] = -lightPos[2] * plane[0];
	m_ShadowMatrix[1][2] = -lightPos[2] * plane[1];
	m_ShadowMatrix[2][2] = dot - lightPos[2] * plane[2];
	m_ShadowMatrix[3][2] = -lightPos[2] * plane[3];
	m_ShadowMatrix[0][3] = -plane[0];
	m_ShadowMatrix[1][3] = -plane[1];
	m_ShadowMatrix[2][3] = -plane[2];
	m_ShadowMatrix[3][3] = dot;

	auto t = glm::translate(glm::mat4(1), glm::vec3(1, 2, 3));

	return glm::transpose(m_ShadowMatrix);
	return m_ShadowMatrix;
}
