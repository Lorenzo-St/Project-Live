#pragma once
#include "structs.h"
int bossPatterns(enemy boss, player pl, bullet** bullets);
int enemyShoot(enemy* en, CP_Sound* bulletSounds, bullet** bullets, player pl);
int fireAtPlayer(enemy a, bullet** bullets, player pl, CP_Sound* bulletSounds);
int playerFire(player pl, bullet** bullets);