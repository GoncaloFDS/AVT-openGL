#pragma once
#include "Camera.h"
class FollowCamera : public Camera {
public:
	FollowCamera(glm::vec3 startingPosition, glm::vec3 center, glm::vec3 up);
	~FollowCamera();

	virtual void OnUpdate(SceneNode& parent) override;

};

