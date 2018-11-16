#include "LensFlare.h"

glm::vec2 LensFlare::convertToScreenSpace(glm::vec3 Worldpos, glm::mat4 viewProjMatrix)
{
	glm::vec4 coord = glm::vec4(Worldpos, 1.0f);

	coord = viewProjMatrix * coord;

	if (coord.w == NULL) return glm::vec2(NULL);

	float x = ((coord.x / coord.w) + 1) / 2.0f;
	float y = 1 - (coord.y / coord.w + 1) / 2.0f;

	return glm::vec2(x, y);

}

LensFlare::LensFlare(Shader *shader, std::vector<lens_flare_model> lens_flares)
{
	m_Shader = shader;
	spacing = 1.0f;

	flares.clear();
	flares.reserve(lens_flares.size());

	for (auto &p : lens_flares)
	{
		flares.emplace_back(p);
	}
}

LensFlare::~LensFlare()
{
	flares.clear();
}

void LensFlare::onRender(Camera *camera, glm::vec3 sunPos)
{
	glm::vec2 sun_coords = convertToScreenSpace(sunPos, camera->GetViewProjMatrix());
	if (sun_coords.x == NULL || sun_coords.y == NULL) return;

	glm::vec2 dist = glm::vec2(0.5) - sun_coords;
	float brightness = 1 - (dist.length() / 0.6f);

	if (brightness > 0)
	{
		m_Shader->Bind();
		for (unsigned int i = 0; i < flares.size(); i++)
		{
			glm::vec2 direction = i * spacing * dist;
			flares[i].position = direction + sun_coords;
			
			auto mvp = camera->GetProjMatrix() * camera->GetViewMatrix() * glm::translate(glm::mat4(1), glm::vec3(flares[i].position, 0)) * glm::scale(glm::mat4(1), glm::vec3(flares[i].scale));
			m_Shader->SetUniformMat4f("MVPMat", mvp);
			flares[i].model.Draw(*m_Shader);
		}

		m_Shader->Unbind();
	}

}
