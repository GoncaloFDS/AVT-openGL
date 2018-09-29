#pragma once

#include <map>
#include <vector>
#include <memory>

#include "InputBind.h"

struct Mouse {
	float posX;
	float posY;
	float deltaX;
	float deltaY;
};

class InputHandler {
private:
	std::map<int, std::vector<std::pair<float, InputBind&>>> m_Inputs;
	std::unique_ptr<Mouse> m_Mouse;

public:
	bool pressed;
	InputHandler();

	void OnKeyDown(int keyCode, bool isRepeat);
	void OnKeyUp(int keyCode, bool isRepeat);

	void OnMouseMove(float posX, float posY);

	void AddKeyControl(int keyCode, InputBind& inputControl, float weight = 1.0f);
	void AddMouseControl(int mouseButton, InputBind& inputControl, float weight = 1.0f);

	float GetMousePosX();
	float GetMousePosY();
	float GetMouseDeltaX();
	float GetMouseDeltaY();

private:
	void UpdateInput(int keyCode, float dir, bool isRepeat);
};
