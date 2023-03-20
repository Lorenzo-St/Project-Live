#include "structs.h"
#include "cprocessing.h"
#include "addStuff.h"
#include "checkStuff.h"
#include "debugInfo.h"
#include "globalData.h"
#include "playerInv.h"
#include "gameLoop.h"
#include <math.h>



int moveEnemies(enemy** en, building *buildings)
{
  enemy* preve = *en;
  enemy* cur = *en;
  while (cur)
  {
    if (cur->dir.x <= 10 && cur->dir.y <= 10)
    {
      cur->dir.x = CP_Random_RangeFloat(-100, 100);
      cur->dir.y = CP_Random_RangeFloat(-100, 100);
    }

    player* p = returnPlayer();
    CP_Vector dir = { p->x - cur->x, p->y - cur->y };
    float mag = sqrtf(dir.x * dir.x + dir.y * dir.y);
    dir.x /= mag;
    dir.y /= mag;
    int check = checkAgainstBuilding(buildings, 1, cur);
    float dt = CP_System_GetDt();
    float magnitude = sqrtf(cur->dir.x * cur->dir.x + cur->dir.y * cur->dir.y);
    if (dt > 1)
      dt = .3f;
    cur->dir.x /= magnitude;
    cur->dir.y /= magnitude;
    cur->dir.x += dir.x / 2.0f;
    cur->dir.y += dir.y / 2.0f;
    magnitude = sqrtf(cur->dir.x * cur->dir.x + cur->dir.y * cur->dir.y);
    cur->dir.x /= magnitude;
    cur->dir.y /= magnitude;
    cur->dir.x *= (check == 1 || check == 3) ? 0 : cur->speed;
    cur->dir.y *= (check == 2 || check == 3) ? 0 : cur->speed;
    cur->x += cur->dir.x * dt;
    cur->y += cur->dir.y * dt;
    cur->dir.x -= cur->dir.x * dt;
    cur->dir.y -= cur->dir.y * dt;

    //drawDebugLine(cur->x, cur->x + cur->dir[0], cur->y, cur->y + cur->dir[0]);
    preve = cur;
    cur = cur->next;
  }
  return 0;
}

int moveBullets(bullet** bullets, enemy** en, player *pl, item *items, building buildings[])
{
  int count = 0;
  //bullet** head = bullets;
  bullet* current = *bullets;
  bullet* prev = *bullets;
  if (current == NULL)
    return -1;
  bool freed = false;

  while (current)
  {
    count++;
    freed = false;
    int check = checkAgainstBuilding(buildings, 2, current);
    current->x += (current->dir[0] * current->speed);
    current->y += (current->dir[1] * current->speed);
    current->life -= CP_System_GetDt();
    if (current->life <= 0 || check)
    { 
      freed = true;
      goto end;
    }
    float distance = sqrtf((current->x - pl->x) * (current->x - pl->x) + (current->y - pl->y) * (current->y - pl->y));
    if (distance < pl->playerRadius && current->users == 0)
    {
      if (pl->powerUp != 5)
      {
        pl->health -= current->pwr;
      }
      freed = true;
      goto end;
    }
    else if (current->users != 1)
      goto end;
    enemy* cur = *en;
    enemy* preve = *en;
    while (cur && !freed) 
    {
      float distance2 = sqrtf((current->x - cur->x) * (current->x - cur->x) + (current->y - cur->y) * (current->y - cur->y));
      if (distance2 > cur->radius / 2.0f)
      {
        preve = cur;
        cur = cur->next;
        continue;
      }
      cur->health -= current->pwr;

      freed = true;
      if (cur->health > 0)
        break;

      pl->kills++;
      if (CP_Random_RangeInt(0, 100) > /*75*/ 100)
        break;
      float temploc[2] = { cur->x, cur->y };
      dropItem(temploc, items);
      enemy* last = cur;
      if (cur == *en)
        *en = cur->next;
      preve->next = last->next;
      cur = last->next;
      removeEnemy(last);
      break;
    }

    end:

    if (!freed) {
      prev = current;
      current = current->next;
    }
    else 
    {
      bullet* last = current;
      if (last == *bullets)
        *bullets = current->next;
      prev->next = current->next;
      current = current->next;
      removeBullet(last);
    }
  }

  if (count > getBulletCount())
    setBulletCount(count);

  return 0;
}
