#pragma once

#include "glm/glm.hpp"

enum class Projection {
	Orthographic = 0, Perspective
};

enum class Direction {
	Front = 0, Right, Up
};


class Camera {
private:
	glm::vec3 m_Position;

	Projection m_ProjectionType;
	glm::mat4 m_ProjMatrix;
	glm::mat4 m_ViewMatrix;

	glm::vec3 m_Front;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_WorldUp;

	float m_MovementSpeed;
	float m_MouseSensivity;
	//float m_Zoom;

	float m_Yaw;
	float m_Pitch;

public:
	Camera(Projection proj, float aspectRatio, glm::vec3 startingPosition, glm::vec3 center);
	~Camera();

	void LookAt(glm::vec3 center);
	void SetAspectRatio(float aspectRatio);
	void Translate(Direction dir, float amount);
	void Rotate(Direction dir, float amount);

	glm::mat4 GetViewProjMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjMatrix();
	glm::vec3 GetPosition();

	void ProcessMouseMovement(float deltaX, float deltaY);
	
private:
	void UpdateCameraVectors();
};

