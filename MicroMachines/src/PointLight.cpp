#include "PointLight.h"
#include "Shader.h"
#include "Camera.h"
#include <string> 


PointLight::PointLight()
	:Light() {
}


PointLight::~PointLight() {
}

void PointLight::UpdateShader(Shader& shader) {
	shader.Bind();
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isEnabled", isEnabled);
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].position", transform.position);
	shader.Unbind();
}

void PointLight::SetupShader(Shader& shader) {
	shader.Bind();
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isEnabled", isEnabled);
	shader.SetUniform1i("Lights[" + std::to_string(ID) + "].isLocal", true);
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].ambient", glm::vec3(.1f, .1f, .1f));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.SetUniform3fv("Lights[" + std::to_string(ID) + "].position", transform.position);
	shader.SetUniform1f("Lights[" + std::to_string(ID) + "].constantAttenuation", 0.02f);
	shader.SetUniform1f("Lights[" + std::to_string(ID) + "].linearAttenuation", 0.01f);
	shader.SetUniform1f("Lights[" + std::to_string(ID) + "].quadraticAttenuation", 0.001f);
	shader.Unbind();
}
