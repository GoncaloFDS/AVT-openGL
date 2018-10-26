#include "DirectionalLight.h"
#include "Shader.h"

DirectionalLight::DirectionalLight(glm::vec3 dir)
	: Light(), m_Direction(dir) {

}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::UpdateShader(Shader& shader) {
	shader.Bind();
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isEnabled", isEnabled);
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].direction", m_Direction);
	shader.Unbind();
}

void DirectionalLight::SetDirection(glm::vec3 dir) {
	m_Direction = dir;
}

void DirectionalLight::SetupShader(Shader& shader) {
	shader.Bind();
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isEnabled", isEnabled);
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isLocal", false);
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].direction", m_Direction);
	shader.Unbind();
}
