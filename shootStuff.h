#pragma once
#ifdef CPP
#include <vector>
#endif
#include "structs.h"

#ifdef CPP
int enemyShoot(std::vector<enemy> const& e, CP_Sound* bulletSounds, bullet** bullets, player* pl);
#endif
int fireAtPlayer(enemy* a, bullet** bullets, player* pl, CP_Sound* bulletSounds);
void runWeaponPattern(weaponData* weapon, CP_Vector* start, CP_Vector* direction, bullet** bullets, int shooter);


