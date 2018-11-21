#pragma once

#include "glm/glm.hpp"
#include "SceneNode.h"
#include "Common.h"

enum class Projection {
	Orthographic = 0, Perspective
};


class Camera : public SceneNode {
protected:

	Projection m_ProjectionType;
	glm::mat4 m_ProjMatrix;
	glm::mat4 m_ViewMatrix;

	glm::mat4 m_ForwardOffset;

	float m_AspectRatio;
	float m_Yaw;
	float m_Pitch;
	float m_MovementSpeed;
	float m_MouseSensivity;

	glm::mat4 m_ViewProjMatrix;
	bool m_IsRenderingShadows;
	glm::mat4 m_ShadowMatrix;
public:
	Camera(glm::vec3 startingPosition, glm::vec3 center, glm::vec3 up);
	~Camera();

	virtual void OnUpdate(SceneNode& parent) override;

	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
	void SetAspectRatio(float aspectRatio);


	void SetProjectionType(Projection projection);
	virtual void Translate(Direction dir, float amount);
	void InvertPitch();

	glm::mat4 GetViewProjMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjMatrix();
	glm::mat4 GetShadowMatrix();
	glm::vec3 GetPosition();
	void SetShadowMatrix(glm::mat4 mat);
	virtual void ProcessMouseMovement(float deltaX, float deltaY);
	glm::vec3 GetWorldUpVector();
	void SetIsRenderingShadows(bool b);

private:
	void CalculateProjectionMatrix(float aspectRatio);
};

