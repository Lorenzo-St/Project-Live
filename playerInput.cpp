#include "cprocessing.h"
#include "playerInput.h"
#include <vector>
#include <array>
#define SCREEN_WIDTH  CP_System_GetWindowWidth()
#define SCREEN_HEIGHT CP_System_GetWindowHeight()
typedef struct InputBinding 
{
  InputBinding(void) : action(Confirm), type(Key), binding(KEY_ENTER) { }
  InputBinding(InputAction a, type t, union bindings b) : action(a), type(t), binding(b) { }

  InputAction action;
  type type;
  union bindings binding;
}InputBinding;

std::vector<InputBinding> keybindings;

std::array<InputBinding,4> positiveAxis;
std::array<InputBinding,4> negativeAxis;


bool isPressed(InputAction action) 
{
  if (action == Any)
  {

    for (int c = CP_GAMEPAD(0); c < CP_GAMEPAD(13); ++c)
      if (CP_Input_GamepadDown(static_cast<CP_GAMEPAD>(c)))
        return true;
    if (CP_Input_KeyDown(KEY_ANY))
      return true;
   
    return false;
  }
  for (auto& binding : keybindings)
  {
    if (binding.action != action)
      continue;
    if (binding.type == Key && CP_Input_KeyDown(binding.binding.key))
      return true;
    if (binding.type == tMouse && CP_Input_MouseDown(binding.binding.mouseB))
      return true;
    if (binding.type == Controller && CP_Input_GamepadDown(binding.binding.button))
      return true;
  }
  return false;
}

bool isTriggered(InputAction action)
{
  if (action == Any)
  {
    for (int c = CP_GAMEPAD(0); c < CP_GAMEPAD(13); ++c)
      if (CP_Input_GamepadTriggered(static_cast<CP_GAMEPAD>(c)))
        return true;
    if (CP_Input_KeyTriggered(KEY_ANY))
      return true;

    return false;
  }
  for (auto& binding : keybindings)
  {
    if (binding.action != action)
      continue;
    if (binding.type == Key && CP_Input_KeyTriggered(binding.binding.key))
      return true;
    if (binding.type == tMouse && CP_Input_MouseTriggered(binding.binding.mouseB))
      return true;
    if (binding.type == Controller && CP_Input_GamepadTriggered(binding.binding.button))
      return true;
  }
  return false;
}

float getXRawDirectional(JoyStick stick)
{
  if (stick == None)
  {
    if (CP_Input_GamepadDown(positiveAxis[1].binding.button))
      return 1;
    else if (CP_Input_GamepadDown(negativeAxis[1].binding.button))
      return -1;
   
    if (CP_Input_KeyDown(positiveAxis[0].binding.key))
      return 1;
    else if (CP_Input_KeyDown(negativeAxis[0].binding.key))
      return -1;
    
    return 0;
  }

  if (stick == JoyStick::Mouse)
    return CP_Input_GetMouseX() - ((SCREEN_WIDTH / 2.0f));


  CP_Vector rawAxis = ((stick == Left) ? (CP_Input_GamepadLeftStick()) : (CP_Input_GamepadRightStick()));
  return rawAxis.x;
}

float getYRawDirectional(JoyStick stick)
{
  if (stick == None)
  {
    if (CP_Input_GamepadDown(positiveAxis[3].binding.button))
      return 1;
    else if (CP_Input_GamepadDown(negativeAxis[3].binding.button))
      return -1;
    
    if (CP_Input_KeyDown(positiveAxis[2].binding.key))
      return 1;
    else if (CP_Input_KeyDown(negativeAxis[2].binding.key))
      return -1;

    return 0;
  }

  if (stick == JoyStick::Mouse)
    return CP_Input_GetMouseY() - ((SCREEN_HEIGHT / 2.0f));
  
  CP_Vector rawAxis = ((stick == Left) ? (CP_Input_GamepadLeftStick()) : (CP_Input_GamepadRightStick()));
  return rawAxis.y;
}

void addBinding(InputAction action, type t, union bindings binding) 
{
  keybindings.push_back({ action, t, binding });
}

void addAxisBinding(bool axis, bool positive, type t, union bindings binding) 
{
  switch (t)
  {
  case Controller:
    if (axis)
    {
      if (positive)
        positiveAxis[1].binding = binding;
      else
        negativeAxis[1].binding = binding;

      positiveAxis[1].action = XAxisPos;
      positiveAxis[1].type = Axis;
      negativeAxis[1].action = XAxisNeg;
      negativeAxis[1].type = Axis;
    }
    else
    {
      if (positive)
        positiveAxis[3].binding = binding;
      else
        negativeAxis[3].binding = binding;

      positiveAxis[3].action = YAxisPos;
      positiveAxis[3].type = Axis;
      negativeAxis[3].action = YAxisNeg;
      negativeAxis[3].type = Axis;
    }
    break;
  case Key:
    if (axis)
    {
      if (positive)
        positiveAxis[0].binding = binding;
      else
        negativeAxis[0].binding = binding;

      positiveAxis[0].action = XAxisPos;
      positiveAxis[0].type = Axis;
      negativeAxis[0].action = XAxisNeg;
      negativeAxis[0].type = Axis;
    }
    else
    {
      if (positive)
        positiveAxis[2].binding = binding;
      else
        negativeAxis[2].binding = binding;
      positiveAxis[2].action = YAxisPos;
      positiveAxis[2].type = Axis;
      negativeAxis[2].action = YAxisNeg;
      negativeAxis[2].type = Axis;
    }

    break;
  }
}
