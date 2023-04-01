#include "cprocessing.h"
#include <math.h>
#include <stdbool.h>

float roundFloat(float x)
{
  if (x < 0)
  {
    x -= .5f;
    x = (float)((int)x);
  }
  else
  {
    x += .5f;
    x = (float)((int)x);
  }
  return x;
}

float Vector2DDot(const CP_Vector* var1, const CP_Vector* var2) 
{
  return var1->x * var2->x + var1->y * var2->y;
}

void Vector2DAdd(CP_Vector* dest, const CP_Vector* var1, const CP_Vector* var2) 
{
  dest->x = var1->x + var2->x;
  dest->y = var1->y + var2->y;
}

void Vector2DScale(CP_Vector* dest, const CP_Vector* var1, float scaler) 
{
  dest->x = var1->x * scaler;
  dest->y = var1->y * scaler;
}

void Vector2DSub(CP_Vector* dest, const CP_Vector* var1, const CP_Vector* var2) 
{
  dest->x = var1->x - var2->x;
  dest->y = var1->y - var2->y;
}

void Vector2DNormalize(CP_Vector* dest, const CP_Vector* var1) 
{
  float magnitude = sqrtf(var1->x * var1->x + var1->y * var1->y);
  dest->x = var1->x / magnitude;
  dest->y = var1->y / magnitude;
}

void Vector2DScaleAdd(CP_Vector* dest, const CP_Vector* var1, const CP_Vector* var2, float scaler) 
{
  dest->x = (var1->x * scaler) + var2->x;
  dest->y = (var1->y * scaler) + var2->y;
}

bool Vector2DEquals(const CP_Vector* var1, const CP_Vector* var2) 
{
  if (var1->x != var2->x)
    return false;
  if (var1->y != var2->y)
    return false;
  return true;
}