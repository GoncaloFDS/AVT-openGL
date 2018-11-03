#pragma once
#include "SceneNode.h"

class Billboard : public SceneNode {
private:
	SceneNode* m_Target;
public:
	Billboard(glm::vec3 position, SceneNode* target);
	~Billboard() = default;

	virtual void OnUpdate(SceneNode& parent) override;
	virtual void OnCollision(SceneNode& other) override;

};