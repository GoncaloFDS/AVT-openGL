#pragma once

#include "glm/glm.hpp"

class InputControl {
public:
	InputControl();
	void AddAmt(float amtToAdd);
	float GetAmt();

private:
	float amt;
};

inline InputControl::InputControl() : amt(0.0f) {}

inline void InputControl::AddAmt(float amtToAdd) { amt += amtToAdd; }

inline float InputControl::GetAmt() { 
	return glm::clamp(amt, -1.0f, 1.0f); 
}
