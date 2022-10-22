#pragma once
#include "structs.h"

int checkKeys(player pl, float multiplier, bullet* bullets, int* playerKeys, int isPaused);
int checkItems(item* items, player pl, enemy* en, enemy* bosses, string* pickupText);