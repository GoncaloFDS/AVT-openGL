#include "gameEventHandler.h"
#include "inputControl.h"

#define MOUSE_OFFSET 512

void GameEventHandler::OnKeyDown(unsigned char keyCode, bool isRepeat) {
  UpdateInput(keyCode, 1.0f, isRepeat);
}

void GameEventHandler::OnKeyUp(unsigned char keyCode, bool isRepeat) {
  UpdateInput(keyCode, -1.0f, isRepeat);
}

void GameEventHandler::AddKeyControl(unsigned char keyCode, InputControl& inputControl, float weight /*= 1.0f*/) {
  m_Inputs[keyCode].emplace_back(weight, inputControl);
}

void GameEventHandler::UpdateInput(unsigned char inputCode, float dir, bool isRepeat) {
  if (isRepeat) return;

  for (auto& i : m_Inputs[inputCode]) {
    i.second.AddAmt(i.first * dir);
  }
}

