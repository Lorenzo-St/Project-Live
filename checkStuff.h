#pragma once
#include "structs.h"

int checkKeys             (player* pl, float *multiplier, bullet* bullets, int* playerKeys, int isPaused, int check);
int checkItems            (item* items, player* pl, enemy* en, enemy* bosses, string* pickupText);
int checkInsideBuilding   (building buildings[NUMBER_OF_BUILDINGS], int which, ...);
int checkAgainstBuilding  (building buildings[NUMBER_OF_BUILDINGS], int which, ...);