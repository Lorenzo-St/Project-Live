#pragma once
#include "cprocessing.h"

typedef enum InputAction 
{
  Any,
  Shoot,
  Inventory,
  Wheel,
  Reload,
  Confirm,
  Return,
  Back,
  Dash,
  XAxisPos,
  XAxisNeg,
  YAxisPos,
  YAxisNeg,
  TopCap
}InputAction;

typedef enum type
{
  Key,
  tMouse,
  Controller,
  Axis
}type;

typedef enum JoyStick
{
  None = -1,
  Mouse,
  Left,
  Right
}JoyStick;

union bindings
{
  bindings(void) : button(CP_GAMEPAD(0)), key(CP_KEY(0)), mouseB(CP_MOUSE(0))
  {
  
  }
  bindings(bindings const& b) 
  {
    key = b.key;
  }
  bindings(CP_MOUSE m)
  {
    mouseB = m;
  }
  bindings(CP_KEY k)
  {
    key = k;
  }
  bindings(CP_GAMEPAD g)
  {
    button = g;
  }
  CP_KEY key;
  CP_GAMEPAD button;
  CP_MOUSE mouseB;
};

float getXRawDirectional(JoyStick stick);
float getYRawDirectional(JoyStick stick);
bool isTriggered(InputAction action);
bool isPressed(InputAction action);
void addBinding(InputAction action, type t, union bindings binding);
void addAxisBinding(bool axis, bool positive, type t, union bindings binding);

