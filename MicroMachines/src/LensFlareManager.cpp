#include "LensFlareManager.h"

LensFlareManager::LensFlareManager(float space, Shader *shader)
{
	m_Shader = shader;
	spacing = space;
	isEnabled = true;

	Model* lens1_model = new Model("res/models/lens flare/lens1.obj");
	Model* lens2_model = new Model("res/models/lens flare/lens2.obj");
	Model* lens3_model = new Model("res/models/lens flare/lens3.obj");
	Model* lens4_model = new Model("res/models/lens flare/lens4.obj");
	Model* lens5_model = new Model("res/models/lens flare/lens5.obj");
	Model* lens6_model = new Model("res/models/lens flare/lens6.obj");
	Model* lens7_model = new Model("res/models/lens flare/lens7.obj");
	Model* lens8_model = new Model("res/models/lens flare/lens8.obj");
	Model* lens9_model = new Model("res/models/lens flare/lens9.obj");

	flares.emplace_back(new LensFlare(*lens6_model, 5.0f));
	flares.emplace_back(new LensFlare(*lens4_model, 2.3f));
	flares.emplace_back(new LensFlare(*lens2_model, 1.0f));
	flares.emplace_back(new LensFlare(*lens7_model, 0.5f));
	flares.emplace_back(new LensFlare(*lens1_model, 0.2f));
	flares.emplace_back(new LensFlare(*lens3_model, 0.6f));
	flares.emplace_back(new LensFlare(*lens9_model, 1.2f));
	flares.emplace_back(new LensFlare(*lens5_model, 0.7f));
	flares.emplace_back(new LensFlare(*lens1_model, 0.12f));
	flares.emplace_back(new LensFlare(*lens7_model, 2.0f));
	flares.emplace_back(new LensFlare(*lens9_model, 1.0f));
	flares.emplace_back(new LensFlare(*lens3_model, 0.7f));
	flares.emplace_back(new LensFlare(*lens5_model, 3.0f));
	flares.emplace_back(new LensFlare(*lens4_model, 4.0f));
	flares.emplace_back(new LensFlare(*lens8_model, 6.0f));
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

void LensFlareManager::OnRender(Camera *camera, Camera *hudCamera, glm::vec3 sunPos, Window *window)
{
	glm::vec2 sun_coords = convertToScreenSpace(sunPos, camera->GetViewProjMatrix());
	if (sun_coords.x == NULL || sun_coords.y == NULL) return;

	glm::vec2 dist = glm::vec2(0.5) - sun_coords;
	float brightness = 1 - (glm::length(dist) / 0.6f);
	glm::vec3 cameraFW = camera->GetForwardVector();

	if (brightness > 0 && cameraFW.x > 0 && cameraFW.z > 0)
	{
		m_Shader->Bind();
		for (unsigned int i = 0; i < flares.size(); i++)
		{
			glm::vec2 pos = (i + 6) * spacing * dist + sun_coords;
			pos.x = (pos.x * window->GetWidth()) - window->GetWidth() / 2;
			pos.y = (pos.y * window->GetHeight()) - window->GetHeight() / 2;

			auto mvp = hudCamera->GetProjMatrix() * hudCamera->GetViewMatrix() * glm::translate(glm::mat4(1), glm::vec3(pos.x, 0, pos.y)) * glm::scale(glm::mat4(1), glm::vec3(30 * flares[i]->getScale()));
			m_Shader->SetUniformMat4f("MVPMat", mvp);
			m_Shader->SetUniform1f("brightness", brightness);
			flares[i]->drawLensFlare(m_Shader);
		}

		m_Shader->Unbind();
	}
}

void LensFlareManager::Toogle() {
	isEnabled = !isEnabled;
}

bool LensFlareManager::IsEnabled()
{
	return isEnabled;
}
