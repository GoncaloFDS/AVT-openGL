#include "LensFlare.h"

LensFlare::LensFlare(Model &model, float scale)
{
	m_Model = &model;
	m_Scale = scale;
}

LensFlare::~LensFlare()
{

}

void LensFlare::setScreenPosition(glm::vec2 new_pos)
{
	m_Position = new_pos;
}

float LensFlare::getScale()
{
	return m_Scale;
}

glm::vec2 LensFlare::getScreenPosition()
{
	return m_Position;
}

void LensFlare::drawLensFlare(Shader *shader)
{
	m_Model->Draw(*shader);
}

