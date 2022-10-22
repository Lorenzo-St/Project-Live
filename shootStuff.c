#include "structs.h"
#include "cprocessing.h"

int fireAtPlayer(enemy a, bullet* bullets, player pl, CP_Sound *bulletSounds)
{
  float dir[2] = { 0,0 };
  dir[0] = pl.x - a.x;
  dir[1] = pl.y - a.y;
  float magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
  dir[0] /= magnitude;
  dir[1] /= magnitude;
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active == 0)
    {
      bullets[i].radius = 10;
      bullets[i].active = 1;
      bullets[i].x = a.x;
      bullets[i].y = a.y;
      bullets[i].dir[0] = dir[0];
      bullets[i].dir[1] = dir[1];
      bullets[i].life = 10;
      bullets[i].pwr = CP_Random_RangeInt(5, 25);
      bullets[i].users = 0;
      bullets[i].speed = BULLET_SPEED;
      CP_Sound_PlayAdvanced(bulletSounds[0], 2.0f, CP_Random_RangeFloat(3.0f, 10.0f), 0, CP_SOUND_GROUP_0);
      break;
    }
  }
  return 0;
}



int bossPatterns(enemy boss, player pl, bullet *bullets)
{
  float dir[2] = { 0 };
  float angle = 0;
  float magnitude = 0;
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
      for (int k = 0; k < MAX_BULLETS; k++)
      {
        if (bullets[k].active == 0)
        {
          bullets[i].radius = 10;
          bullets[k].active = 1;
          bullets[k].x = boss.x;
          bullets[k].y = boss.y;
          bullets[k].dir[0] = dir[0];
          bullets[k].dir[1] = dir[1];
          bullets[k].life = 10;
          bullets[k].pwr = CP_Random_RangeInt(5, 25);
          bullets[k].users = 0;
          bullets[k].speed = BULLET_SPEED;
          break;
        }
      }
    }
    break;
  case 1:

    for (int k = 0; k < SPREAD_COUNT * 3; k++)
    {
      angle = CP_Random_RangeFloat(-100, 100);
      dir[0] = pl.x - boss.x + angle;
      dir[1] = pl.y - boss.y + angle;
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      for (int i = 0; i < MAX_BULLETS; i++)
      {
        if (bullets[i].active == 0)
        {
          bullets[i].radius = 10;
          bullets[i].active = 1;
          bullets[i].x = boss.x;
          bullets[i].y = boss.y;
          bullets[i].dir[0] = dir[0];
          bullets[i].dir[1] = dir[1];
          bullets[i].life = 5;
          bullets[i].users = 0;
          bullets[i].pwr = 20;
          bullets[i].speed = BULLET_SPEED;
          break;
        }
      }
    }

    break;
  case 2:

    dir[0] = pl.x - boss.x;
    dir[1] = pl.y - boss.y;
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    for (int i = 0; i < MAX_BULLETS; i++)
    {

      if (bullets[i].active == 0)
      {
        bullets[i].radius = 10;
        bullets[i].active = 1;
        bullets[i].x = boss.x;
        bullets[i].y = boss.y;
        bullets[i].dir[0] = dir[0];
        bullets[i].dir[1] = dir[1];
        bullets[i].life = 10;
        bullets[i].pwr = CP_Random_RangeInt(5, 25);
        bullets[i].users = 0;
        bullets[i].speed = BULLET_SPEED;
        break;
      }

    }
    break;
  case 3:

    dir[0] = (pl.x - boss.x);
    dir[1] = (pl.y - boss.y);
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
      if (bullets[i].active == 0)
      {
        bullets[i].radius = 50;
        bullets[i].active = 1;
        bullets[i].x = boss.x;
        bullets[i].y = boss.y;
        bullets[i].dir[0] = dir[0];
        bullets[i].dir[1] = dir[1];
        bullets[i].life = 10;
        bullets[i].pwr = CP_Random_RangeInt(75, 100);
        bullets[i].users = 0;
        bullets[i].speed = BULLET_SPEED / 2.0f;
        break;
      }
    }

    break;
  }
  return 0;
}
int playerFire(player pl, bullet *bullets)
{
  float dir[2] = { 0,0 };
  float magnitude = 0;
  float angle = 0;
  if (pl.weapon == 3)
  {

    for (int k = 0; k < SPREAD_COUNT; k++)
    {
      angle = CP_Random_RangeFloat(-100, 100);
      dir[0] = (CP_Input_GetMouseX() - CP_System_GetWindowWidth() / 2.0f) - pl.x + angle;
      dir[1] = -(CP_Input_GetMouseY() - CP_System_GetWindowHeight() / 2.0f) - pl.y + angle;
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      for (int i = 0; i < MAX_BULLETS; i++)
      {
        if (bullets[i].active == 0)
        {
          bullets[i].radius = 10;
          bullets[i].active = 1;
          bullets[i].x = pl.x;
          bullets[i].y = pl.y;
          bullets[i].dir[0] = dir[0];
          bullets[i].dir[1] = dir[1];
          bullets[i].life = 5;
          bullets[i].users = 1;
          if (pl.powerUp != 1)
            bullets[i].pwr = 20;
          else if (pl.powerUp == 1)
            bullets[i].pwr = 20000;
          bullets[i].speed = BULLET_SPEED;
          break;
        }
      }
    }
  }
  else
  {
    dir[0] = (CP_Input_GetMouseX() - CP_System_GetWindowWidth() / 2.0f) - pl.x;
    dir[1] = -(CP_Input_GetMouseY() - CP_System_GetWindowHeight() / 2.0f) - pl.y;
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
      if (bullets[i].active == 0)
      {
        bullets[i].radius = 10;
        bullets[i].active = 1;
        bullets[i].x = pl.x;
        bullets[i].y = pl.y;
        bullets[i].dir[0] = dir[0];
        bullets[i].dir[1] = dir[1];
        bullets[i].life = 5;
        bullets[i].users = 1;
        bullets[i].speed = BULLET_SPEED;
        switch (pl.weapon)
        {
        case 0:
          if (pl.powerUp != 1)
            bullets[i].pwr = 25;
          else if (pl.powerUp == 1)
            bullets[i].pwr = 20000;
          break;
        case 1:
          if (pl.powerUp != 1)
            bullets[i].pwr = 15;
          else if (pl.powerUp == 1)
            bullets[i].pwr = 20000;

          break;
        case 2:
          if (pl.powerUp != 1)
            bullets[i].pwr = 300;
          else if (pl.powerUp == 1)
            bullets[i].pwr = 20000;
          bullets[i].speed *= 4;
          break;
        case 4:
          if (pl.powerUp != 1)
            bullets[i].pwr = 30;
          else if (pl.powerUp == 1)
            bullets[i].pwr = 20000;
          break;
        }
        break;
      }
    }
  }
  return 0;
}
int bossShoot(enemy* bosses, player pl, bullet *bullets)
{
  for (int i = 0; i < MAX_BOSSESS; i++)
  {
    if (bosses[i].active == 0)
      continue;
    if (bosses[i].cooldown <= 0)
    {
      bossPatterns(bosses[i], pl, bullets);
      switch (bosses[i].pattern)
      {
      case 0:
        bosses[i].cooldown = .75f;
        break;
      case 1:
        bosses[i].cooldown = 1.0f;
        break;
      case 2:
        bosses[i].cooldown = .1f;
        break;
      case 3:
        bosses[i].cooldown = 2.0f;
        break;
      }

    }
    float d = CP_System_GetDt();
    if (d > 1)
      bosses[i].cooldown -= .03f;
    else
      bosses[i].cooldown -= d;
  }
  return 0;
}

int enemyShoot(enemy* en, CP_Sound* bulletSounds, bullet *bullets, player pl)
{
  for (int i = 0; i < MAX_ENEMIES; i++)
  {
    if (en[i].active == 0)
      continue;
    if (en[i].cooldown <= 0)
    {
      fireAtPlayer(en[i], bullets, pl, bulletSounds);
      switch (en[i].type)
      {
      case 0:
        en[i].cooldown = 5.0f;
        break;
      case 1:
        en[i].cooldown = .5f;
        break;
      }
    }
    float d = CP_System_GetDt();
    if (d > 1)
      en[i].cooldown -= .03f;
    else
      en[i].cooldown -= d;
  }
  return 0;
}