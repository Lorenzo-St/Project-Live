#pragma once
#include "structs.h"

int dropItem(float loc[], item* items);
int addPickup(item a, notiString* pickupText);


void    removeEnemy    (enemy* toRemove);
void    removeBullet   (bullet* toRemove);
enemy*  addEnemy       (enemy** head);
enemy*  setEnemyStats  (enemy* e, camera c, int type);
bullet* addBullet      (bullet** head);