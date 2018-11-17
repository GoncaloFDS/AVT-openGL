#include "LensFlareManager.h"

LensFlareManager::LensFlareManager(float space, Shader *shader, std::vector<LensFlare*> &lens_flares)
{
	m_Shader = shader;
	spacing = space;

	flares.clear();
	flares.reserve(lens_flares.size());

	for (auto p : lens_flares) flares.emplace_back(p);
}

LensFlareManager::~LensFlareManager()
{
	flares.clear();
}

glm::vec2 LensFlareManager::convertToScreenSpace(glm::vec3 Worldpos, glm::mat4 viewProjMatrix)
{
	glm::vec4 coord = glm::vec4(Worldpos, 1.0f);

	coord = viewProjMatrix * coord;

	if (coord.w == NULL) return glm::vec2(NULL);

	float x = (coord.x / coord.w + 1) / 2.0f;
	float y = 1 - (coord.y / coord.w + 1) / 2.0f;

	return glm::vec2(x, y);
}

void LensFlareManager::onRender(Camera *camera, Camera *hudCamera, glm::vec3 sunPos, Window *window)
{
	glm::vec2 sun_coords = convertToScreenSpace(sunPos, camera->GetViewProjMatrix());
	if (sun_coords.x == NULL || sun_coords.y == NULL) return;

	glm::vec2 dist = glm::vec2(0.5) - sun_coords;
	float brightness = 1 - (std::sqrt(std::pow(dist.x, 2) + std::pow(dist.y, 2)) / 0.7f);

	if (brightness > 0)
	{
		m_Shader->Bind();
		for (unsigned int i = 0; i < flares.size(); i++)
		{
			glm::vec2 pos = i * spacing * dist + sun_coords;
			pos.x = (pos.x * window->GetWidth()) - window->GetWidth() / 2;
			pos.y = (pos.y * window->GetHeight()) - window->GetHeight() / 2;
	
			auto mvp = hudCamera->GetProjMatrix() * hudCamera->GetViewMatrix() * glm::translate(glm::mat4(1), glm::vec3(pos.x, 0, pos.y)) * glm::scale(glm::mat4(1), glm::vec3(20 * flares[i]->getScale()));
			m_Shader->SetUniformMat4f("MVPMat", mvp);
			m_Shader->SetUniform1f("brightness", brightness);
			flares[i]->drawLensFlare(m_Shader);
		}

		m_Shader->Unbind();
	}
}
