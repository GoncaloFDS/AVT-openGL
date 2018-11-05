#include "SpotLight.h"
#include "Shader.h"



SpotLight::SpotLight()
	: Light() {
	m_Forward = glm::vec3(0, 0, 1);
}


SpotLight::~SpotLight() {

}

void SpotLight::UpdateShader(Shader& shader) {
	shader.Bind();
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isEnabled", isEnabled);
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].position", m_WorldMatrix * glm::vec4(transform.position, 1));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].direction", m_WorldMatrix * glm::vec4(m_Forward, 0));
	shader.Unbind();
}

void SpotLight::SetupShader(Shader& shader) {
	shader.Bind();
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isEnabled", isEnabled);
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isLocal", true);
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isSpot", true);
	//shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].ambient", glm::vec3(.1f, .1f, .1f));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	//shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].position", m_WorldMatrix * glm::vec4(transform.position, 1));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].direction", m_WorldMatrix * glm::vec4(m_Forward, 0));
	shader.SetUniform1f("Lights[" + std::to_string(ID) + "].constantAttenuation", 2.0f);
	shader.SetUniform1f("Lights[" + std::to_string(ID) + "].linearAttenuation", 0.0001f);
	shader.SetUniform1f("Lights[" + std::to_string(ID) + "].quadraticAttenuation", 0.00002f);
	shader.SetUniform1f("Lights[" + std::to_string(ID) + "].spotCutoff", glm::cos(glm::radians(10.5f)));
	shader.SetUniform1f("Lights[" + std::to_string(ID) + "].spotOutterCutOff", glm::cos(glm::radians(25.5f)));
	shader.Unbind();
}
