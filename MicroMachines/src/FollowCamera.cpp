#include "FollowCamera.h"


FollowCamera::FollowCamera(glm::vec3 startingPosition, glm::vec3 center, glm::vec3 up)
	: Camera(startingPosition, center, up) {

}

FollowCamera::~FollowCamera() {
}

void FollowCamera::OnUpdate(SceneNode& parent) {
	glm::vec3 springarm = parent.transform.rotation * glm::vec4(glm::normalize(glm::vec3(0, -1, 5)), 1);
	transform.position = parent.transform.position - springarm * 50.0f;
	m_Forward = glm::rotate(glm::mat4(1), m_Yaw, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), m_Pitch, m_Right) * glm::vec4(springarm, 0);
	m_Forward = glm::normalize(m_Forward);
	m_Right = glm::cross(m_Forward, m_WorldUp);
	m_Up = glm::cross(m_Right, m_Forward);

	Camera::OnUpdate(parent);

}
