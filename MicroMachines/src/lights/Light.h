#pragma once
#include "SceneNode.h"
class Light : public SceneNode {

protected:
	static int numberOfLights;
	int ID;
	bool isEnabled = true;

public:
	Light();
	~Light();

	virtual void UpdateShader(Shader& shader) = 0;
	virtual void SetupShader(Shader& shader) = 0;
	virtual void ToogleLight();
};

