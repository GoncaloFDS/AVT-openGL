#include "DebugCamera.h"
#include "Timer.h"


DebugCamera::DebugCamera(glm::vec3 startingPosition, glm::vec3 center, glm::vec3 up)
	: Camera(startingPosition, center, up) {
	m_Forward = glm::normalize(center - transform.position);
	m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
	m_Up = glm::cross(m_Right, m_Forward);
	m_MouseSensivity *= 0.016f;

}

DebugCamera::~DebugCamera() {
}

void DebugCamera::OnUpdate(SceneNode& parent) {
	m_ViewMatrix = glm::lookAt(transform.position, transform.position + m_Forward, m_WorldUp);
	m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;

	for (auto node : m_ChildNodes)
		node->OnUpdate(parent); //skip the camera

}

void DebugCamera::ProcessMouseMovement(float deltaX, float deltaY) {
	deltaX *= -m_MouseSensivity;
	deltaY *= m_MouseSensivity;

	m_Forward = glm::rotate(glm::mat4(1), deltaX, glm::vec3(0, 1, 0)) * 
		glm::rotate(glm::mat4(1), deltaY, m_Right) * 
		glm::vec4(m_Forward, 0);
	m_Forward = glm::normalize(m_Forward);
	m_Right = glm::cross(m_Forward, m_WorldUp);
	m_Up = glm::cross(m_Right, m_Forward);
	
	
}

void DebugCamera::DetachFrom(Camera camera) {
	transform.position = camera.transform.position;
	m_Forward = camera.GetForwardVector();
	m_Up = camera.GetUpVector();
	m_Right = camera.GetRightVector();
}

