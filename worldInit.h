#pragma once
#include "structs.h"
#include "cprocessing.h"

int initPlayer(player *pl, float* multiplier, float* addTimer);
int initStrings(string *pickupText);
int initBosses(enemy *bosses);
void initAudio(CP_Sound *bulletSounds);
int initEnemies(enemy *en);
int initBullets(bullet *bullets);
int initDrops(item *items);