#pragma once
float roundFloat(float x);
float Vector2DDot(const CP_Vector* var1, const CP_Vector* var2);
void Vector2DAdd(CP_Vector* dest, const CP_Vector* var1, const CP_Vector* var2);
void Vector2DScale(CP_Vector* dest, const CP_Vector* var1, float scaler);
void Vector2DSub(CP_Vector* dest, const CP_Vector* var1, const CP_Vector* var2);
void Vector2DNormalize(CP_Vector* dest, const CP_Vector* var1);
void Vector2DScaleAdd(CP_Vector* dest, const CP_Vector* var1, const CP_Vector* var2, float scaler);
bool Vector2DEquals(const CP_Vector* var1, const CP_Vector* var2);
