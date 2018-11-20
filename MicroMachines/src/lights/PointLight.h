#pragma once
#include "Light.h"

class Shader;
class Camera;

class PointLight : public Light {
public:
	PointLight();
	~PointLight();

	virtual void UpdateShader(Shader& shader) override;

	virtual void SetupShader(Shader& shader) override;
};

