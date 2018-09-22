#pragma once

#include <map>
#include <vector>

class InputControl;

class GameEventHandler {
private:
  std::map<unsigned char, std::vector<std::pair<float, InputControl&>>> m_Inputs;

public:
  GameEventHandler(){}
  void OnKeyDown(unsigned char keyCode, bool isRepeat);
  void OnKeyUp(unsigned char keyCode, bool isRepeat);
  
  void AddKeyControl(unsigned char keyCode, InputControl& inputControl,
                     float weight = 1.0f);
 
private:
  void UpdateInput(unsigned char, float dir, bool isRepeat);
};
