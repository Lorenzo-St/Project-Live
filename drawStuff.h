#pragma once
#include "structs.h"

void  initScrollable       (void);
void  releaseScrollable    (void);
void  drawBackGroundLayer  (void);
void  setContexts          (bool ya);
void  drawPlayer           (player pl, camera c);
void  drawWeapon           (int weapon, float powerUpTimer, int powerUp);
void  drawBullets          (bullet *bullets, camera C);
void  drawEnemies          (enemy **en, camera C);
void  drawPickupText       (notiString *pickupText, camera C);
void  drawItems            (item *items, camera C);
void  drawBuildings        (building* buildings, camera c);
void  drawWheel            (player* p);
void  drawAmmo             (player* p, bool inv, bool wheel);
void  drawWheelImages      (float x, float y, float radius);
void  drawArc              (float startAngle, float endAngle, float x, float y, float radius);

int   drawInventory        (InvItem* head);
