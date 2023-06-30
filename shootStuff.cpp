#include <Vector>
#include "playerInfo.h"
extern "C"
{

#define CPP
#include "structs.h"
#include "cprocessing.h"
#include "playerInv.h"
#include "addStuff.h"
#include "gameLoop.h"
#include "Sound.h"
#include "drawStuff.h"
#include <math.h>

  bullet* setBulletStats(bullet* thisOne, float dir[2], int user, float pl[2])
  {
    thisOne->radius = 10 * (SCREEN_WIDTH / 1920.0f);
    thisOne->x = pl[0];
    thisOne->y = pl[1];
    thisOne->dir[0] = dir[0];
    thisOne->dir[1] = dir[1];
    thisOne->life = 5;
    thisOne->users = user;
    return thisOne;
  }


  void runWeaponPattern(weaponData* weapon, CP_Vector* start, CP_Vector* direction, bullet** bullets, int shooter)
  {
    float dir[2] = { direction->x, direction->y };
    float angle = 0;
    float magnitude = 0;
    bullet* thisOne = nullptr;
    float pos[2] = {
    start->x, start->y
    };
    switch (weapon->pattern)
    {
    case 0:
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      thisOne = addBullet(bullets);
  
      thisOne = setBulletStats(thisOne, dir, shooter, pos);
      if (thisOne == NULL)
        return;
      thisOne->pwr = weapon->damage;
      thisOne->speed = weapon->bulletSpeed;
      break;
    case 1:
      for (int k = 0; k < SPREAD_COUNT; k++)
      {
        angle = CP_Random_RangeFloat(-.333f, .333f);

        
        dir[0] += cosf(angle);
        dir[1] += sinf(angle);
        magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
        dir[0] /= magnitude;
        dir[1] /= magnitude;
        thisOne = addBullet(bullets);
        thisOne = setBulletStats(thisOne, dir, shooter ,pos);
        if (thisOne == NULL)
          return;
        thisOne->pwr = weapon->damage;
        thisOne->speed = weapon->bulletSpeed;
      }
      break;
    }
  }

  int fireAtPlayer(enemy* a, bullet** bullets, player* pl, CP_Sound* bulletSounds)
  {
    CP_Vector pos = *PlayerGetPos(pl);
    pos.x -= a->pos.x;
    pos.y -= a->pos.y;
    runWeaponPattern(a->weapon, &a->pos,
      &pos,
      bullets, 0);

    return 0;
  }

  int enemyShoot(std::vector<enemy> const& e, CP_Sound* bulletSounds, bullet** bullets, player* pl)
  {
    int i = 0;
    for(auto& enem : e)
    {
      if (enem.alive == false)
      {
        i++;
        continue;
      }
      if (enem.weapon->reloadClock <= 0)
      {
        fireAtPlayer(const_cast<enemy*>(&enem), bullets, pl, bulletSounds);
        enem.weapon->reloadClock = enem.weapon->attackSpeed;
      }
      float d = CP_System_GetDt();
      if (d > 1)
        enem.weapon->reloadClock -= .03f;
      else
        enem.weapon->reloadClock -= d;
      i++;
    }
    return 0;
  }
}