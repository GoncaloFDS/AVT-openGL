#pragma once

#include "glm/glm.hpp"
#include "SceneNode.h"
#include "Common.h"

enum class Projection {
	Orthographic = 0, Perspective
};


class Camera : public SceneNode{
private:

	Projection m_ProjectionType;
	glm::mat4 m_ProjMatrix;
	glm::mat4 m_ViewMatrix;

	//glm::vec3 m_Forward;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_WorldUp;
	glm::mat4 m_ForwardOffset;

	float m_AspectRatio;
	float m_Yaw;
	float m_Pitch;
	float m_MovementSpeed;
	float m_MouseSensivity;

	glm::mat4 m_ViewProjMatrix;
public:
	Camera(glm::vec3 startingPosition, glm::vec3 center, glm::vec3 up);
	~Camera();

	virtual void OnUpdate(SceneNode& parent) override;

	void LookAt(glm::vec3 center);
	void SetAspectRatio(float aspectRatio);


	void SetProjectionType(Projection projection);
	void Translate(Direction dir, float amount);
	

	glm::mat4 GetViewProjMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjMatrix();
	glm::vec3 GetPosition();

	void ProcessMouseMovement(float deltaX, float deltaY);

private:
	void CalculateProjectionMatrix(float aspectRatio);
};

