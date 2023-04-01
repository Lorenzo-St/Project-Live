#include "structs.h"
#include "cprocessing.h"
#include "playerInv.h"
#include "addStuff.h"
#include "gameLoop.h"
#include "Sound.h"
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

int fireAtPlayer(enemy a, bullet** bullets, player pl, CP_Sound *bulletSounds)
{
  float dir[2] = { 0,0 };
  dir[0] = pl.pos.x - a.pos.x;
  dir[1] = pl.pos.y - a.pos.y;
  float magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
  dir[0] /= magnitude;
  dir[1] /= magnitude;
  bullet* thisOne;
  thisOne = addBullet(bullets);
  thisOne = setBulletStats(thisOne, dir, 0, (float[2]) { a.pos.x, a.pos.y });
  if (thisOne == NULL)
    return -1;
  thisOne->pwr = CP_Random_RangeInt(5, 25);
  thisOne->speed = BULLET_SPEED;
  return 0;
}

int bossPatterns(enemy boss, player pl, bullet **bullets)
{
  float dir[2] = { 0 };
  float angle = 0;
  float magnitude = 0;
  bullet* thisOne;
  switch (boss.pattern)
  {
  case 0:
    for (int i = 0; i < BASE_PATTERN; i++)
    {
      dir[0] = cosf(360.0f / BASE_PATTERN * i);
      dir[1] = sinf(360.0f / BASE_PATTERN * i);
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      thisOne = addBullet(bullets);
      thisOne = setBulletStats(thisOne, dir, 0, (float[2]) { boss.pos.x, boss.pos.y });
      if (thisOne == NULL)
        return -1;
      thisOne->pwr = CP_Random_RangeInt(5, 25);
      thisOne->speed = BULLET_SPEED;
    }
    break;
  case 1:

    for (int k = 0; k < SPREAD_COUNT * 2; k++)
    {
      angle = CP_Random_RangeFloat(-100, 100);
      dir[0] = pl.pos.x - boss.pos.x + angle;
      dir[1] = pl.pos.y - boss.pos.y + angle;
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      thisOne = addBullet(bullets);
      thisOne = setBulletStats(thisOne, dir, 0, (float[2]) { boss.pos.x, boss.pos.y });
      if (thisOne == NULL)
        return -1;
      thisOne->pwr = 20;
      thisOne->speed = BULLET_SPEED;
    }

    break;
  case 2:

    dir[0] = pl.pos.x - boss.pos.x;
    dir[1] = pl.pos.y - boss.pos.y;
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    thisOne = addBullet(bullets);
    thisOne = setBulletStats(thisOne, dir, 0, (float[2]) { boss.pos.x, boss.pos.y });
    if (thisOne == NULL)
      return -1;
    thisOne->pwr = CP_Random_RangeInt(5, 25);
    thisOne->speed = BULLET_SPEED;
    break;
  case 3:

    dir[0] = (pl.pos.x - boss.pos.x);
    dir[1] = (pl.pos.y - boss.pos.y);
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    thisOne = addBullet(bullets);
    thisOne = setBulletStats(thisOne, dir, 0, (float[2]) { boss.pos.x, boss.pos.y });
    if (thisOne == NULL)
      return -1;
    thisOne->pwr = CP_Random_RangeInt(5, 25);
    thisOne->speed = BULLET_SPEED;
    thisOne->radius = 50;
    break;
  }
  return 0;
}

int playerFire(player pl, bullet **bullets)
{
  float dir[2] = { 0,0 };
  float magnitude = 0;
  float angle = 0;
  bullet* thisOne;
  if (pl.weapon->reloadClock > 0)
    return 1;
  if (returnInvSel())
    return 0;
  if (retAmmo()->active[returnSelected()]-- <= 0)
  {
    
    retAmmo()->active[returnSelected()] = 0;
    reloadFromReserves();
    return -1;
  }

  if (((returnWheel())[returnSelected()].durability -= (CP_Random_RangeInt(0, 100) > 75))  <= 0) 
  {
    removeFromWheel(returnSelected());
    removeItem(0);
  }

  switch (pl.weapon->pattern) 
  {
  case 0:
    dir[0] = (CP_Input_GetMouseX() - SCREEN_WIDTH / 2.0f);
    dir[1] = -(CP_Input_GetMouseY() - SCREEN_HEIGHT / 2.0f);
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    thisOne = addBullet(bullets);
    thisOne = setBulletStats(thisOne, dir, 1, (float[2]) { pl.pos.x, pl.pos.y });
    if (thisOne == NULL)
      return -1;
    thisOne->pwr = pl.weapon->damage;
    thisOne->speed = pl.weapon->bulletSpeed;
    break;
  case 1:
    for (int k = 0; k < SPREAD_COUNT; k++)
    {
      angle = CP_Random_RangeFloat(-25, 25);
      dir[0] = (CP_Input_GetMouseX() - SCREEN_WIDTH / 2.0f) + angle;
      dir[1] = -(CP_Input_GetMouseY() - SCREEN_HEIGHT / 2.0f) + angle;
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      thisOne = addBullet(bullets);
      thisOne = setBulletStats(thisOne, dir, 1, (float[2]) { pl.pos.x, pl.pos.y });
      if (thisOne == NULL)
        return -1;
      thisOne->pwr = pl.weapon->damage;
      thisOne->speed = pl.weapon->bulletSpeed;
    }
    break;
  }

  CP_Sound s = getWeaponSounds(pl.weapon->type, (returnWheel())[returnSelected()].itemId);
  CP_Sound_Play(s);
  return 0;
}

int enemyShoot(enemy* e, CP_Sound* bulletSounds, bullet **bullets, player pl)
{
  int i = 0;
  while(i < MAX_ENEMIES)
  {
    enemy* en = e + i;
    if (en->alive == false) 
    {
      i++;
      continue;
    }
    if (en->cooldown <= 0)
    {
      
      switch (en->type)
      {
      case 0:
        fireAtPlayer(*en, bullets, pl, bulletSounds);
        en->cooldown = 5.0f;
        break;
      case 1:
        fireAtPlayer(*en, bullets, pl, bulletSounds);
        en->cooldown = .5f;
        break;
      case 2:
        bossPatterns(*en, pl, bullets);
        switch (en->pattern)
        {
        case 0:
          en->cooldown = .75f;
          break;
        case 1:
          en->cooldown = 1.0f;
          break;
        case 2:
          en->cooldown = .1f;
          break;
        case 3:
          en->cooldown = 2.0f;
          break;
        }
        break;
      }
    }
    float d = CP_System_GetDt();
    if (d > 1)
      en->cooldown -= .03f;
    else
      en->cooldown -= d;
    i++;
  }
  return 0;
}