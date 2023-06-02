#include <vector>
extern "C"
{

#include "structs.h"
#include "cprocessing.h"
#include "checkStuff.h"
#include "worldSystems.h"
#include "playerInv.h"
#include "ItemData.h"
#include "gameLoop.h"
#include "EnemyInfo.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

  int addPickup(item* a, const InvItem* b, notiString* pickupText, int count)
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
    bullet* news = reinterpret_cast<bullet*>(calloc(1, sizeof(bullet)));
    if (news == NULL)
      return NULL;
    news->next = *head;
    *head = news;
    return news;
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

  void setEnemyStats(enemy* e, const EnemyInfo* en, camera c, int type, int wave)
  {
    e->pos.x = CP_Random_RangeFloat(c.x - SCREEN_WIDTH / 2.0f, c.x + SCREEN_WIDTH / 2.0f);
    e->pos.y = CP_Random_RangeFloat(c.y - SCREEN_HEIGHT / 2.0f, c.y + SCREEN_HEIGHT / 2.0f);
    e->MaxHealth = (int)(((float)e->MaxHealth * (1.0f + (wave / en->scaler))));
    e->radius *= (SCREEN_WIDTH / 1920.0f);
    e->health = e->MaxHealth;
    return;
  }

  void removeEnemy(enemy* toRemove)
  {
    if (toRemove == NULL)
      return;
    toRemove->alive = 0;
    decreaseAlive();
  }

  int dropItem(float loc[], std::vector<item>& items)
  {
    item toAdd = {};
    toAdd.x = loc[0];
    toAdd.y = loc[1];
    toAdd.id = chooseId();
    toAdd.active = true;
    toAdd.radius = 20;
    toAdd.life = 2.5f;
    bool assigned = false;
    for (auto &item : items) 
    {
      if (item.active)
        continue;
      item = toAdd;
      assigned = true;
      break;
    }

    if(!assigned)
      items.push_back(toAdd);
    return 0;
  }
}