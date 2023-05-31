#pragma once
#include "structs.h"


void loadWeapons();

const weaponData* getWeapon(int id);

void ReleaseWeaponsStorage(void);