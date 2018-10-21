#pragma once

#include "glm/glm.hpp"
#include "glm/common.hpp"
#include "Common.h"

class InputBind {
public:
	InputBind(bool shouldClean);
	InputBind();
	void AddAmt(float amtToAdd);
	float GetAmt();
	bool isPressed();

private:
	float m_Amount;
	bool m_ShouldClean;
};

inline InputBind::InputBind(bool shouldClean) : m_Amount(0.0f), m_ShouldClean(shouldClean) {}
inline InputBind::InputBind() : m_Amount(0.0f), m_ShouldClean(true) {}

inline void InputBind::AddAmt(float amtToAdd) { 
	m_Amount += amtToAdd; 
}

inline float InputBind::GetAmt() { 
// 	auto temp = m_Amount;
// 	if(m_ShouldClean)
// 		m_Amount = 0;
	return glm::clamp(m_Amount, -1.0f, 1.0f);
}

inline bool InputBind::isPressed() {
	auto temp = m_Amount;
	if (m_ShouldClean)
		m_Amount = 0;
	return temp == 1;
}
