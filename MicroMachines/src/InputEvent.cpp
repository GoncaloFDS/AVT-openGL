#include "InputEvent.h"
#include "inputControl.h"
#include <ctype.h>

#define MOUSE_OFFSET 512

void InputEvent::OnKeyDown(unsigned char keyCode, bool isRepeat) {
	UpdateInput(keyCode, 1.0f, isRepeat);
}

void InputEvent::OnKeyUp(unsigned char keyCode, bool isRepeat) {
	UpdateInput(keyCode, -1.0f, isRepeat);
}

void InputEvent::OnMouseInput(int button, int state, int deltaX, int deltaY) {
	
}

void InputEvent::AddKeyControl(unsigned char keyCode, InputControl& inputControl, float weight /*= 1.0f*/) {
	m_Inputs[toupper(keyCode)].emplace_back(weight, inputControl);
}

void InputEvent::AddMouseControl(unsigned char mouseButton, InputControl& inputControl, float weight /*= 1.0f*/) {
	m_Inputs[mouseButton + MOUSE_OFFSET].emplace_back(weight, inputControl);
}

void InputEvent::UpdateInput(unsigned char inputCode, float dir, bool isRepeat) {
	if (isRepeat) return;

	for (auto& i : m_Inputs[toupper(inputCode)]) {
		i.second.AddAmt(i.first * dir);
	}
}
