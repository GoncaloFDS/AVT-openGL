#pragma once
#include "Camera.h"
class DebugCamera : public Camera {
public:
	DebugCamera(glm::vec3 startingPosition, glm::vec3 center, glm::vec3 up);
	~DebugCamera();

	virtual void OnUpdate(SceneNode& parent) override;
	virtual void ProcessMouseMovement(float deltaX, float deltaY) override;

	void DetachFrom(Camera camera);
};

