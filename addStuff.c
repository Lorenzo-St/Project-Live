#include "structs.h"
#include "cprocessing.h"
#include "checkStuff.h"
#include "worldSystems.h"
#include "playerInv.h"
#include "gameLoop.h"
#include <stdio.h>
#include <stdlib.h>


int addPickup(item* a,  InvItem* b, notiString* pickupText, int count)
{
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

enemy* addEnemy(enemy** head) 
{
  enemy* new = (enemy*)malloc(1 * sizeof(enemy));
  if (new == NULL)
    return NULL;
  new->next = *head;
  *head = new;
  increaseAlive();
  return new;
}

enemy* setEnemyStats(enemy* e, camera c, int type) 
{
  building* building = returnBuildings();
  e->type = type;
  switch (type) 
  {
  case 0:
    do 
    {
      e->x = CP_Random_RangeFloat(c.x - SCREEN_WIDTH / 2.0f, c.x + SCREEN_WIDTH / 2.0f);
      e->y = CP_Random_RangeFloat(c.y - SCREEN_HEIGHT / 2.0f, c.y + SCREEN_HEIGHT / 2.0f);
    } while (checkInsideBuilding(building, 1, e));
    
    e->speed = 100;
    e->cooldown = 1.5f;
    e->health = 60;
    e->radius = 25 * (SCREEN_WIDTH / 1920.0f);

    break;
  case 1:
    do
    {
      e->x = CP_Random_RangeFloat(c.x - SCREEN_WIDTH / 2.0f, c.x + SCREEN_WIDTH / 2.0f);
      e->y = CP_Random_RangeFloat(c.y - SCREEN_HEIGHT / 2.0f, c.y + SCREEN_HEIGHT / 2.0f);
    } while (checkInsideBuilding(building, 1, e));

    e->speed = 100;
    e->cooldown = .3f;
    e->health = 50;
    e->radius = 25 * (SCREEN_WIDTH / 1920.0f);
    break;
  case 2:
    do
    {
      e->x = CP_Random_RangeFloat(c.x - SCREEN_WIDTH / 2.0f, c.x + SCREEN_WIDTH / 2.0f);
      e->y = CP_Random_RangeFloat(c.y - SCREEN_HEIGHT / 2.0f, c.y + SCREEN_HEIGHT / 2.0f);
    } while (checkInsideBuilding(building, 1, e));

    e->speed = 75;
    e->cooldown = .2f;
    e->health = 300;
    e->radius = 50 * (SCREEN_WIDTH / 1920.0f);
    break;
  }

  e->MaxHealth = e->health;
  return e;
}

void removeEnemy(enemy* toRemove) 
{
  if (toRemove == NULL)
    return;
  free(toRemove);
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