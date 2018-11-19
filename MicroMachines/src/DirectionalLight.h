#pragma once
#include "Light.h"
#include <glm/vec3.hpp>  

class Shader;


class DirectionalLight : public Light {
private:
	glm::vec3 m_Direction;
public:
	DirectionalLight(glm::vec3 dir);
	~DirectionalLight();

	virtual void UpdateShader(Shader& shader) override;
	virtual void SetupShader(Shader& shader) override;

	void SetDirection(glm::vec3 dir);
	glm::vec3 getDirection();
};

