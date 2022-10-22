#pragma once
#include "structs.h"

int addPickup(item a, string* pickupText);
void addEnemy(int bossesEnabled, enemy* en, enemy* bosses);
int dropItem(float loc[], item* items);