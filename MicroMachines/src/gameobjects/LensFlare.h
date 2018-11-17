#pragma once

#include "Model.h"

class LensFlare
{
	Model *m_Model;
	glm::vec2 m_Position;
	float m_Scale;

public:
	LensFlare(Model &model, float scale);
	~LensFlare();

	void setScreenPosition(glm::vec2 new_pos);
	float getScale();
	glm::vec2 getScreenPosition();
	void drawLensFlare(Shader *shader);
};

