#pragma once

#include "glm/glm.hpp"

class InputBind {
public:
	InputBind();
	void AddAmt(float amtToAdd);
	float GetAmt();

private:
	float amt;
};

inline InputBind::InputBind() : amt(0.0f) {}

inline void InputBind::AddAmt(float amtToAdd) { 
	amt += amtToAdd; 
}

inline float InputBind::GetAmt() { 
	return glm::clamp(amt, -1.0f, 1.0f); 
}
