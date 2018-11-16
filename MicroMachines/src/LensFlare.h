#pragma once

#include "shader.h"
#include "Model.h"
#include "cameras/Camera.h"

struct lens_flare_model
{
	Model model;
	glm::vec2 position;
	float scale;

	lens_flare_model(Model mod, float scal) : model(mod), position(0.0f), scale(scal) {};
};


class LensFlare
{
	Shader *m_Shader;
	std::vector <lens_flare_model> flares;
	float spacing;

	glm::vec2 convertToScreenSpace(glm::vec3 Worldpos, glm::mat4 viewProjMatrix);

public:
	LensFlare(Shader *shader, std::vector<lens_flare_model> lens_flares);
	~LensFlare();

	void onRender(Camera *camera, glm::vec3 sunPos);
};

