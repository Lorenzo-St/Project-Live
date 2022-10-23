#pragma once
#include "structs.h"

void drawPlayer(player pl, camera c);
void drawWeapon(int weapon, float powerUpTimer, int powerUp);
void drawBullets(bullet *bullets, camera C);
void drawEnemies(enemy *en, camera C);
void drawBosses(enemy *bosses, camera C);
void drawPickupText(string *pickupText, camera C);
void drawItems(item *items, camera C);
void drawBuildings(building* buildings, camera c);