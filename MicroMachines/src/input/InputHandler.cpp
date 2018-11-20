#include "InputHandler.h"
#include "InputBind.h"
#include <ctype.h>

#define MOUSE_OFFSET 512

InputHandler::InputHandler() {
	m_Mouse = std::make_unique<Mouse>();
}

void InputHandler::OnKeyDown(int keyCode, bool isRepeat) {
	UpdateInput(keyCode, 1.0f, isRepeat);
}

void InputHandler::OnKeyUp(int keyCode, bool isRepeat) {
	UpdateInput(keyCode, -1.0f, isRepeat);
}

void InputHandler::OnMouseMove(float posX, float posY) {
	if (pressed) {
		m_Mouse->deltaX = posX - m_Mouse->posX;
		m_Mouse->deltaY = posY - m_Mouse->posY;
		if (fabs(m_Mouse->deltaX) <= 1.f)
			m_Mouse->deltaX = 0;
		if (fabs(m_Mouse->deltaY) <= 1.f)
			m_Mouse->deltaY = 0;
	}
	else {
		m_Mouse->deltaX = 0;
		m_Mouse->deltaY = 0;
	}
	m_Mouse->posX = posX;
	m_Mouse->posY = posY;
}

void InputHandler::AddKeyControl(int keyCode, InputBind& inputControl, float weight /*= 1.0f*/) {
	m_Inputs[toupper(keyCode)].emplace_back(weight, inputControl);
}

void InputHandler::AddMouseControl(int mouseButton, InputBind& inputControl, float weight /*= 1.0f*/) {
	m_Inputs[mouseButton + MOUSE_OFFSET].emplace_back(weight, inputControl);
}

float InputHandler::GetMousePosX() {
	return m_Mouse->posX;
}

float InputHandler::GetMousePosY() {
	return m_Mouse->posY;
}

float InputHandler::GetMouseDeltaX() {
	return m_Mouse->deltaX;
}

float InputHandler::GetMouseDeltaY() {
	return m_Mouse->deltaY;
}

void InputHandler::UpdateInput(int inputCode, float dir, bool isRepeat) {
	if (isRepeat) return;

	for (auto& i : m_Inputs[toupper(inputCode)]) {
		i.second.AddAmt(i.first * dir);
	}
}
