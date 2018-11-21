#pragma once

#include "SceneNode.h"
#include "lights/DirectionalLight.h"

class Sun : public SceneNode {
private:
	const static float sun_dist;
	SceneNode* m_Target;
	DirectionalLight *light;
	glm::mat4 m_ShadowMatrix;
public:
	Sun(float scale, glm::vec3 direction, SceneNode* target);
	~Sun() = default;

	virtual void OnUpdate(SceneNode& parent) override;

	void SetTarget(SceneNode* target);
	glm::vec3 GetWorldPosition(glm::vec3 camPos);
	DirectionalLight* GetLight();
	void ToogleLight();

	glm::mat4 GetShadowMatrix();
private:
	glm::vec3 m_lightPos;
};
