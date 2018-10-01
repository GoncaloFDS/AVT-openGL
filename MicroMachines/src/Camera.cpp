#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "common.h"


Camera::Camera(Projection proj, float aspectRatio, glm::vec3 startingPosition, glm::vec3 center) {
	m_ProjMatrix = proj == Projection::Orthographic ?
		glm::ortho(-2.0f * aspectRatio, 2.0f * aspectRatio, -2.0f, 2.0f, 1.0f, 100.0f) :
		glm::perspective(glm::radians(45.0f), aspectRatio, 1.0f, 5000.f);

	m_ProjectionType = proj;

	m_Position = startingPosition;
	m_WorldUp = glm::vec3(0, 1, 0);

	LookAt(center);
	m_Front = glm::normalize(center - m_Position);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::cross(m_Right, m_Front);

	m_Pitch = asin(m_Front.y);
	m_Yaw = atan2(m_Front.x, m_Front.z);
	m_Yaw -= 90;
	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	m_MouseSensivity = 0.1f;
	m_MovementSpeed = 0.5f;
	

}

Camera::~Camera() {

}

void Camera::LookAt(glm::vec3 center) {
	m_ViewMatrix = glm::lookAt(m_Position, center, m_WorldUp);
}

void Camera::SetAspectRatio(float aspectRatio) {
	if (m_ProjectionType == Projection::Orthographic) {
		if (aspectRatio >= 1.0f)
			m_ProjMatrix = glm::ortho(-2.0f * aspectRatio, 2.0f * aspectRatio, -2.0f, 2.0f, 1.0f, 100.0f);
		else
			m_ProjMatrix = glm::ortho(-2.0f, 2.0f, -2.0f/aspectRatio, 2.0f/aspectRatio, 1.0f, 100.0f);
	}
	else {
		m_ProjMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 1.0f, 5000.f);
	}
}

void Camera::Translate(Direction dir, float amount = 1.0f) {
	glm::vec3 directionVec;
	switch (dir) {
		case Direction::Front:
			directionVec = m_Front;
			break;
		case Direction::Right:
			directionVec = m_Right;
			break;
		case Direction::Up:
			directionVec = m_Up;
			break;
	}
	m_Position += directionVec * amount * m_MovementSpeed;
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::Rotate(Direction dir, float amount) {
	glm::vec3 directionVec;
	switch (dir) {
		case Direction::Front:
			ASSERT(false);
			break;
		case Direction::Right:
			directionVec = m_Right;
			break;
		case Direction::Up:
			directionVec = m_Up;
			break;
	}
	auto center = (m_Position + m_Front) + directionVec * amount;
	m_ViewMatrix = glm::lookAt(m_Position, center, m_Up);
}

glm::mat4 Camera::GetViewProjMatrix() {
	return m_ProjMatrix * m_ViewMatrix;
}

glm::mat4 Camera::GetViewMatrix() {
	return m_ViewMatrix;
}

glm::mat4 Camera::GetProjMatrix() {
	return m_ProjMatrix;
}

glm::vec3 Camera::GetPosition() {
	return m_Position;
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
	deltaX *= m_MouseSensivity;
	deltaY *= m_MouseSensivity;

	m_Yaw += deltaX;
	m_Pitch += deltaY;
	

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);
	
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
