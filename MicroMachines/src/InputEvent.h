#pragma once

#include <map>
#include <vector>

class InputControl;

class InputEvent {
private:
  std::map<unsigned char, std::vector<std::pair<float, InputControl&>>> m_Inputs;

public:
  InputEvent(){}
  void OnKeyDown(unsigned char keyCode, bool isRepeat);
  void OnKeyUp(unsigned char keyCode, bool isRepeat);
//   void OnMouseDown(unsigned char mouseButton, int numClicks);
//   void OnMouseUp(unsigned char mouseButton, int numClicks);
  void OnMouseInput(int button, int state, int deltaX, int deltaY);

  void AddKeyControl(unsigned char keyCode, InputControl& inputControl, float weight = 1.0f);
  void AddMouseControl(unsigned char mouseButton, InputControl& inputControl, float weight = 1.0f);
 
private:
  void UpdateInput(unsigned char, float dir, bool isRepeat);
};
