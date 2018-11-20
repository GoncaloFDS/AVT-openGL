#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "common.h"
#include "Timer.h"
#include "glm/gtc/constants.hpp"
#include "glm/vec4.hpp"


Camera::Camera(glm::vec3 startingPosition, glm::vec3 center, glm::vec3 up)
	: SceneNode() {

	m_ProjMatrix = glm::perspective(glm::radians(45.0f), 1080.0f / 720.0f, 1.0f, 5000.f);
	m_ProjectionType = Projection::Perspective;

	transform.position = startingPosition;
	m_WorldUp = up;

	m_ViewMatrix = glm::lookAt(transform.position, center, m_WorldUp);
	m_Forward = glm::normalize(center - transform.position);
	m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
	m_Up = glm::cross(m_Right, m_Forward);

	m_MouseSensivity = 0.1f;
	m_MovementSpeed = 0.5f;
	m_ForwardOffset = glm::mat4(1.0f);
}

Camera::~Camera() {

}

void Camera::OnUpdate(SceneNode& parent) {

	m_ViewMatrix = glm::lookAt(transform.position, transform.position + m_Forward, m_Up);
	m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
	for (auto node : m_ChildNodes)
		node->OnUpdate(parent); //skip the camera
}

void Camera::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
	m_ViewMatrix = glm::lookAt(eye, center, up);
}

void Camera::SetAspectRatio(float aspectRatio) {
	m_AspectRatio = aspectRatio;
	CalculateProjectionMatrix(aspectRatio);
}

void Camera::CalculateProjectionMatrix(float aspectRatio) {
	if (m_ProjectionType == Projection::Orthographic) {
		if (aspectRatio >= 1.0f)
			m_ProjMatrix = glm::ortho(-200.0f * aspectRatio, 200.0f * aspectRatio, -200.0f, 200.0f, 1.0f, 5000.0f);
		else
			m_ProjMatrix = glm::ortho(-200.0f, 200.0f, -200.0f / aspectRatio, 200.0f / aspectRatio, 1.0f, 00.0f);
	}
	else {
		m_ProjMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 1.0f, 5000.f);
	}
}


glm::vec3 Camera::GetWorldUpVector() {
	return m_WorldUp;
}

void Camera::SetProjectionType(Projection projection) {
	m_ProjectionType = projection;
	CalculateProjectionMatrix(m_AspectRatio);
}

void Camera::Translate(Direction dir, float amount = 1.0f) {
	glm::vec3 directionVec;
	switch (dir) {
	case Direction::Front:
		directionVec = m_Forward;
		break;
	case Direction::Right:
		directionVec = m_Right;
		break;
	case Direction::Up:
		directionVec = m_Up;
		break;
	}
	transform.position += directionVec * amount * m_MovementSpeed;
}


void Camera::InvertPitch() {
// 	transform.position.y = -transform.position.y;
// 	m_Forward.y = -m_Forward.y;
// 	m_Right = -m_Right;
// 	m_Up.x = -m_Up.x;
// 	m_Up.z = -m_Up.z;
	
	//m_ViewMatrix = glm::lookAt(transform.position, transform.position + m_Forward, m_WorldUp);
	m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(1, -1, 1));
	m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}

glm::mat4 Camera::GetViewProjMatrix() {
	return m_ViewProjMatrix;
}

glm::mat4 Camera::GetViewMatrix() {
	return m_ViewMatrix;
}

glm::mat4 Camera::GetProjMatrix() {
	return m_ProjMatrix;
}

glm::vec3 Camera::GetPosition() {
	return transform.position;
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
	deltaX *= -m_MouseSensivity * Timer::deltaTime;
	deltaY *= m_MouseSensivity * Timer::deltaTime;

	m_Pitch += deltaY;
	m_Yaw += deltaX;

	m_Right = glm::cross(m_Forward, m_WorldUp);
	m_Up = glm::cross(m_Right, m_Forward);
}

