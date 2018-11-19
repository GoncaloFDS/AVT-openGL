#pragma once

#include "SceneNode.h"
#include "DirectionalLight.h"

class Sun : public SceneNode {
private:
	const static float sun_dist;
	SceneNode* m_Target;
	DirectionalLight *light;
public:
	Sun(float scale, glm::vec3 direction, SceneNode* target);
	~Sun() = default;

	virtual void OnUpdate(SceneNode& parent) override;
	void SetTarget(SceneNode* target);
	glm::vec3 getWorldPosition(glm::vec3 camPos);
	DirectionalLight* getLight();
	void toogleLight();
};
