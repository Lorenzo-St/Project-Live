#pragma once
#include "structs.h"

bool checkMouseBoxCollide     (float x, float y, float width, float height);
bool checkKeys                (player* pl, float* multiplier, bullet** bullets, bool* InvOpen, bool* wheelOpen, int isPaused, int check);
bool checkMouseArcCollide     (float startAngle, float endAngle, float centerX, float centerY, float radius);
bool checkInsideBuilding      (building* buildings, int which, ...);
int  checkAgainstBuilding     (building* buildings, int which, ...);
int  checkItems               (item* items, player* pl, enemy* en, notiString* pickupText);
void checkAlphanumericKeys    (char* dest, int* pos, int max);
void verifyEnemyCount         (enemy* e);
CP_Vector checkLineCollision  (CP_Vector* Line1Start, CP_Vector* Line1End, CP_Vector* Line2Start, CP_Vector* Line2End);
