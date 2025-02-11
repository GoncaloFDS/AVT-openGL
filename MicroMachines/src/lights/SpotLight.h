#pragma once
#include "Light.h"

class Shader;
class SpotLight : public Light {
public:
	SpotLight();
	~SpotLight();

	virtual void UpdateShader(Shader& shader) override;
	virtual void SetupShader(Shader& shader) override;
};

