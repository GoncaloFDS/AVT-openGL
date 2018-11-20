#pragma once

#include <iostream>

#include "shader.h"
#include "Window.h"
#include "cameras/Camera.h"
#include "gameobjects/LensFlare.h"

class LensFlareManager
{
private:
	Shader *m_Shader;
	std::vector <LensFlare*> flares;
	float spacing;
	bool isEnabled;

	glm::vec2 convertToScreenSpace(glm::vec3 Worldpos, glm::mat4 viewProjMatrix);

public:
	LensFlareManager(float space, Shader *shader);
	~LensFlareManager();
	void OnRender(Camera *camera, Camera *hudCamera, glm::vec3 sunPos, Window *window);
	void Toogle();
	bool IsEnabled();
};
