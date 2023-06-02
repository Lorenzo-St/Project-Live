#include <vector>
extern"C"
{
#define CPP

#include "structs.h"
#include "cprocessing.h"
#include "addStuff.h"
#include "checkStuff.h"
#include "debugInfo.h"
#include "globalData.h"
#include "playerInv.h"
#include "gameLoop.h"
#include <math.h>



  int moveEnemies(std::vector<enemy>& en, building* buildings)
  {
    int i = 0;
    for(auto & e:en)
    {
      if (e.alive == false)
      {
        continue;
      }
      if (e.dir.x <= 10 && e.dir.y <= 10)
      {
        e.dir.x = CP_Random_RangeFloat(-100, 100);
        e.dir.y = CP_Random_RangeFloat(-100, 100);
        player* p = returnPlayer();
        CP_Vector dir = { p->pos.x - e.pos.x, p->pos.y - e.pos.y };
        float mag = sqrtf(dir.x * dir.x + dir.y * dir.y);

        dir.x /= mag;
        dir.y /= mag;
        float magnitude = sqrtf(e.dir.x * e.dir.x + e.dir.y * e.dir.y);
        e.dir.x /= magnitude;
        e.dir.y /= magnitude;
        if (mag > 50)
        {
          e.dir.x += dir.x;
          e.dir.y += dir.y;
        }
        magnitude = sqrtf(e.dir.x * e.dir.x + e.dir.y * e.dir.y);
        e.dir.x /= magnitude;
        e.dir.y /= magnitude;
        e.dir.x *= e.speed;
        e.dir.y *= e.speed;

      }
      float dt = CP_System_GetDt();
      if (dt > 1)
        dt = .3f;
      if (checkInsideBuilding(buildings, 1, e))
      {
        e.OOBCOunt++;
        if (e.OOBCOunt >= 2 * CP_System_GetFrameRate())
        {
          removeEnemy(&e);
        }
      }
      int check = checkAgainstBuilding(buildings, 1, &e);
      e.pos.x += e.dir.x * dt * ((check == 1 || check == 3) ? -1 : 1);
      e.pos.y += e.dir.y * dt * ((check == 2 || check == 3) ? -1 : 1);
      e.dir.x -= e.dir.x * dt;
      e.dir.y -= e.dir.y * dt;
      i++;

      //drawDebugLine(e.x, e.x + e.dir[0], e.y, e.y + e.dir[0]);
    }
    return 0;
  }

  int moveBullets(bullet** bullets, std::vector<enemy>& en, player* pl, std::vector<item>& items, building buildings[])
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
      for(auto &e : en)
      {
        if (e.alive == false)
        {
          continue;
        }

        float distance2 = sqrtf((current->x - e.pos.x) * (current->x - e.pos.x) + (current->y - e.pos.y) * (current->y - e.pos.y));
        if (distance2 > e.radius / 2.0f)
        {
          continue;
        }
        e.health -= current->pwr;

        freed = true;
        if (e.health > 0)
          break;

        pl->kills++;
        if (CP_Random_RangeInt(0, 100) > /*75*/ 100)
          break;
        float temploc[2] = { e.pos.x, e.pos.y };
        dropItem(temploc, items);
        removeEnemy(&e);
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
}
