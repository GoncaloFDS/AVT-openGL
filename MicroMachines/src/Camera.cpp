#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "common.h"
#include "Timer.h"
#include "glm/gtc/constants.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/constants.hpp"


Camera::Camera(glm::vec3 startingPosition, glm::vec3 center, glm::vec3 up) 
	: SceneNode(){

	m_ProjMatrix = glm::perspective(glm::radians(45.0f), 1080.0f/720.0f, 1.0f, 5000.f);
	m_ProjectionType = Projection::Perspective;

	transform.position = startingPosition;
	m_WorldUp = up;

	LookAt(center);
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
	
	glm::vec3 springarm = parent.transform.rotation * glm::vec4(glm::normalize(glm::vec3(0, -1, 5)), 1);
	transform.position = parent.transform.position - springarm * 50.0f;
	m_Forward = glm::rotate(glm::mat4(1), m_Yaw, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), m_Pitch, m_Right) * glm::vec4(springarm, 0);
	m_Forward = glm::normalize(m_Forward);
	m_Right = glm::cross(m_Forward, m_WorldUp);
	m_Up = glm::cross(m_Right, m_Forward);
	m_ViewMatrix = glm::lookAt(transform.position, transform.position + m_Forward, m_Up);
	m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
	for (auto node : m_ChildNodes)
		node->OnUpdate(parent); //skip the camera
}

void Camera::LookAt(glm::vec3 center) {
	m_ViewMatrix = glm::lookAt(transform.position, center, m_WorldUp);
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
	//m_ViewMatrix = glm::lookAt(transform.position, transform.position + m_Forward, m_Up);
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

	//m_ForwardOffset = glm::rotate(m_ForwardOffset, deltaX, m_Up);
	//m_ForwardOffset = glm::rotate(m_ForwardOffset, deltaY, m_Right);

	m_Pitch += deltaY;
	m_Yaw += deltaX;

	//m_Forward = glm::rotate(glm::mat4(1.0f), deltaX, m_Up) * glm::vec4(m_Forward, 1);
	//m_Forward = glm::rotate(glm::mat4(1.0f), deltaY, m_Right) * glm::vec4(m_Forward, 1);
	//m_Forward = glm::normalize(m_Forward);
	m_Right = glm::cross(m_Forward, m_WorldUp);
	m_Up = glm::cross(m_Right, m_Forward);
}

