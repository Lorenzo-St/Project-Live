#include "structs.h"
#include "cprocessing.h"
#include "checkStuff.h"
#include "worldSystems.h"
#include "playerInv.h"
#include "gameLoop.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int addPickup(item* a,  InvItem* b, notiString* pickupText, int count)
{
  if (b->itemId == -1 || b->count < 0)
    return -1;
  for (int i = 0; i < MAX_TEXT; i++)
  {
    if (pickupText[i].active)
      continue;

    pickupText[i].active = 1;
    pickupText[i].life = 1.5f;
    pickupText[i].x = a->x;
    pickupText[i].y = a->y;
    snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Aquired: %s x%i", b->name, count);
  }
  return 0;
}

bullet* addBullet(bullet** head) 
{
  bullet* new = (bullet*)malloc(1 * sizeof(bullet));
  if (new == NULL)
    return NULL;
  new->next = *head;
  *head = new;
  return new;
}

void removeBullet(bullet* toRemove) 
{
  if (toRemove == NULL)
    return;
  free(toRemove);

}

enemy* addEnemy(enemy* head) 
{
  int i = 0;
  while (i < MAX_ENEMIES)
  {
    if ((head + i)->alive == 0) 
    {
      (head + i)->alive = true;
      break;
    }

    i++;
  }
  increaseAlive();
  return (head + i);
}

enemy* setEnemyStats(enemy* e, camera c, int type, int wave) 
{
  e->type = type;
  e->pos.x = CP_Random_RangeFloat(c.x - SCREEN_WIDTH / 2.0f, c.x + SCREEN_WIDTH / 2.0f);
  e->pos.y = CP_Random_RangeFloat(c.y - SCREEN_HEIGHT / 2.0f, c.y + SCREEN_HEIGHT / 2.0f);

  switch (type) 
  {
  case 0:


    e->speed = 100;
    e->cooldown = 1.5f;
    e->health = (int)(60 * (1.0f + (wave / 50.0f)));
    e->radius = 25 * (SCREEN_WIDTH / 1920.0f);

    break;
  case 1:


    e->speed = 100;
    e->cooldown = .3f;
    e->health = (int)(45 * (1.0f + (wave / 75.0f)));
    e->radius = 25 * (SCREEN_WIDTH / 1920.0f);
    break;
  case 2:
    e->speed = 75;
    e->cooldown = .2f;
    e->health = (int)(300 * (1.0f + (wave / 25.0f)));
    e->radius = 60 * (SCREEN_WIDTH / 1920.0f);
    e->pattern = type;
    break;
  }

  e->MaxHealth = e->health;
  return e;
}

void removeEnemy(enemy* toRemove) 
{
  if (toRemove == NULL)
    return;
  toRemove->alive = 0;
  decreaseAlive();
}

int dropItem(float loc[], item* items)
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
    {
      items[i].x = loc[0];
      items[i].y = loc[1];
      items[i].type = CP_Random_RangeInt(0, 4);
      switch (items[i].type)
      {
      case 0:
        items[i].containes = CP_Random_RangeInt(20, 50);
        break;
      case 1:
        items[i].containes = CP_Random_RangeInt(0, 1);
        break;
      case 2:
        /* Fall through */
      case 3:
        /* Fall through */
      case 4:
        items[i].type = 2;
        items[i].containes = CP_Random_RangeInt(1, 10);
        break;
      }
      items[i].active = 1;
      items[i].life = 10;
      return 0;
    }
  }
  return 0;
}