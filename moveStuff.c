#include "structs.h"
#include "cprocessing.h"
#include "addStuff.h"
#include "checkStuff.h"
#include "debugInfo.h"
#include "globalData.h"
#include "playerInv.h"
#include "gameLoop.h"
#include <math.h>



int moveEnemies(enemy* en, building *buildings)
{
  int i = 0;
  while (i < MAX_ENEMIES)
  {
    enemy* cur = en + i;
    if (cur->alive == false)
    {
      i++;
      continue;
    }
    if (cur->dir.x <= 10 && cur->dir.y <= 10)
    {
      cur->dir.x = CP_Random_RangeFloat(-100, 100);
      cur->dir.y = CP_Random_RangeFloat(-100, 100);
      player* p = returnPlayer();
      CP_Vector dir = { p->pos.x - cur->pos.x, p->pos.y - cur->pos.y };
      float mag = sqrtf(dir.x * dir.x + dir.y * dir.y);
 
      dir.x /= mag;
      dir.y /= mag;
      float magnitude = sqrtf(cur->dir.x * cur->dir.x + cur->dir.y * cur->dir.y);
      cur->dir.x /= magnitude;
      cur->dir.y /= magnitude;
      if (mag > 50)
      {
        cur->dir.x += dir.x;
        cur->dir.y += dir.y;
      }
      magnitude = sqrtf(cur->dir.x * cur->dir.x + cur->dir.y * cur->dir.y);
      cur->dir.x /= magnitude;
      cur->dir.y /= magnitude;
      cur->dir.x *= cur->speed;
      cur->dir.y *= cur->speed;

    }
    float dt = CP_System_GetDt();
    if (dt > 1)
        dt = .3f;
    if (checkInsideBuilding(buildings, 1, cur)) 
    {
      cur->OOBCOunt++;
      if (cur->OOBCOunt >= 2 * CP_System_GetFrameRate()) 
      {
        removeEnemy(cur);
      }
    }
    int check = checkAgainstBuilding(buildings, 1, cur);
    cur->pos.x += cur->dir.x * dt * ((check == 1 || check == 3) ? -1 : 1);
    cur->pos.y += cur->dir.y * dt * ((check == 2 || check == 3) ? -1 : 1);
    cur->dir.x -= cur->dir.x * dt;
    cur->dir.y -= cur->dir.y * dt;
    i++;

    //drawDebugLine(cur->x, cur->x + cur->dir[0], cur->y, cur->y + cur->dir[0]);
  }
  return 0;
}

int moveBullets(bullet** bullets, enemy* en, player *pl, item *items, building buildings[])
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
    float distance = sqrtf((current->x - pl->pos.x) * (current->x - pl->pos.x) + (current->y - pl->pos.y) * (current->y - pl->pos.y));
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
    int i = 0;
    while (i < MAX_ENEMIES) 
    {
      enemy* cur = en + i;
      if (cur->alive == false) 
      {
        i++;
        continue;
      }

      float distance2 = sqrtf((current->x - cur->pos.x) * (current->x - cur->pos.x) + (current->y - cur->pos.y) * (current->y - cur->pos.y));
      if (distance2 > cur->radius / 2.0f)
      {
        i++;
        continue;
      }
      cur->health -= current->pwr;

      freed = true;
      if (cur->health > 0)
        break;

      pl->kills++;
      if (CP_Random_RangeInt(0, 100) > /*75*/ 100)
        break;
      float temploc[2] = { cur->pos.x, cur->pos.y };
      dropItem(temploc, items);
      removeEnemy(cur);
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
