#pragma once
#include "structs.h"
#include "cprocessing.h"

int initBuildings(building* buildings);
int initPlayer(player *pl, float* multiplier, float* addTimer);
int initStrings(notiString *pickupText);
void initAudio(CP_Sound *bulletSounds);
int initBullets(bullet *bullets);
int initDrops(item *items);