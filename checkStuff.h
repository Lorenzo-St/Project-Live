#pragma once
#include "structs.h"

bool checkMouseBoxCollide  (float x, float y, float width, float height);
bool checkKeys             (player* pl, float* multiplier, bullet* bullets, int* playerKeys, bool* InvOpen, bool* wheelOpen, int isPaused, int check);
bool checkMouseArcCollide  (float startAngle, float endAngle, float centerX, float centerY, float radius);
bool checkInsideBuilding   (building buildings[NUMBER_OF_BUILDINGS], int which, ...);
bool checkAgainstBuilding  (building buildings[NUMBER_OF_BUILDINGS], int which, ...);
int checkItems(item* items, player* pl, enemy* en, enemy* bosses, string* pickupText);