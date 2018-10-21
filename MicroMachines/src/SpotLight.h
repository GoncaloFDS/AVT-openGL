#pragma once
#include "Light.h"

class Shader;
class SpotLight : public Light {
public:
	SpotLight();
	~SpotLight();

	virtual void UpdateShader(Shader& shader) override;

	void SetupShader(Shader& shader);
};

