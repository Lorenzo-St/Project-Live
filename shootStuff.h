#pragma once
#ifdef CPP
#include <vector>
#endif
#include "structs.h"

int bossPatterns(enemy* boss, player pl, bullet** bullets);
#ifdef CPP
int enemyShoot(std::vector<enemy> const& e, CP_Sound* bulletSounds, bullet** bullets, player pl);
#endif
int fireAtPlayer(enemy a, bullet** bullets, player pl, CP_Sound* bulletSounds);
int playerFire(const player* pl, bullet** bullets);