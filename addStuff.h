#pragma once
#include "structs.h"
typedef struct EnemyInfo EnemyInfo;
int dropItem(float loc[], item* items);
int addPickup(item* a, InvItem* b, notiString* pickupText, int count);


void    removeEnemy    (enemy* toRemove);
void    removeBullet   (bullet* toRemove);
void    setEnemyStats  (enemy* e, const EnemyInfo* en, camera c, int type, int wave);
bullet* addBullet      (bullet** head);