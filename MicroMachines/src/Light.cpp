#include "Light.h"



int Light::numberOfLights;

Light::Light() {
	ID = numberOfLights;
	numberOfLights = numberOfLights + 1;
	isEnabled = true;
}


Light::~Light() {
}

void Light::ToogleLight() {
	isEnabled = !isEnabled;
}
