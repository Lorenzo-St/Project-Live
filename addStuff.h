#pragma once
#include "structs.h"

int dropItem(float loc[], item* items);
int addPickup(item* a, InvItem* b, notiString* pickupText, int count);


void    removeEnemy    (enemy* toRemove);
void    removeBullet   (bullet* toRemove);
enemy*  addEnemy       (enemy* head);
enemy*  setEnemyStats  (enemy* e, camera c, int type, int wave);
bullet* addBullet      (bullet** head);