#pragma once
#include "structs.h"
typedef struct EnemyInfo EnemyInfo;
#ifdef CPP
int dropItem(float loc[], std::vector<item>& items);
#endif
int addPickup(item* a, const InvItem* b, notiString* pickupText, int count);


void    removeEnemy    (enemy* toRemove);
void    removeBullet   (bullet* toRemove);
void    setEnemyStats  (enemy* e, const EnemyInfo* en, camera c, int type, int wave);
bullet* addBullet      (bullet** head);