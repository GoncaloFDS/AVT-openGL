#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(Projection proj, glm::vec3 startingPosition) {
	m_ProjMatrix = proj == Projection::Orthographic ?
		glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 100.0f) :
		glm::perspective(glm::radians(45.0f), 1080.f / 720.f, 1.0f, 5000.f);

	m_Position = startingPosition;
	m_WorldUp = glm::vec3(0, 1, 0);

	LookAt(glm::vec3(0.0f));
	m_Front = glm::normalize(glm::vec3(0.0f) - m_Position);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::cross(m_Right, m_Front);

	m_RotationSpeed = 3.0f;
	m_TranslationSpeed = 0.5f;

}

Camera::~Camera() {

}

void Camera::LookAt(glm::vec3 center) {
	m_ViewMatrix = glm::lookAt(m_Position, center, m_WorldUp);
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
	m_Position += directionVec * amount * m_TranslationSpeed;
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	//m_ViewMatrix = glm::translate(glm::mat4(1.0f), directionVec * amount * m_TranslationSpeed) * m_ViewMatrix;
}

void Camera::Rotate(Direction dir, float amount) {

}

void Camera::Update() {

}