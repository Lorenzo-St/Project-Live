#include "structs.h"
#include "cprocessing.h"
#include "addStuff.h"
#include "checkStuff.h"
#include <math.h>

int moveEnemies(enemy* en, building buildings[])
{
  for (int i = 0; i < MAX_ENEMIES; i++)
  {
    if (en[i].active == 0)
      continue;
    if (en[i].dir[0] <= 10 && en[i].dir[1] <= 10)
    {
      en[i].dir[0] = CP_Random_RangeFloat(-100, 100);
      if (en[i].x + en[i].dir[0] > CP_System_GetWindowWidth() / 2.0 || en[i].x + en[i].dir[0] < -CP_System_GetWindowWidth() / 2.0f)
        en[i].dir[0] = -en[i].dir[0];
      en[i].dir[1] = CP_Random_RangeFloat(-100, 100);
      if (en[i].y + en[i].dir[1] > CP_System_GetWindowHeight() / 2.0 || en[i].y + en[i].dir[1] < -CP_System_GetWindowHeight() / 2.0f)
        en[i].dir[1] = -en[i].dir[1];
    }
    int check = checkAgainstBuilding( buildings, 1, en[i]);
    en[i].x += en[i].dir[0] * CP_System_GetDt() * (check == 0 || check == 2);
    en[i].y += en[i].dir[1] * CP_System_GetDt() * (check == 0 || check == 1);
    en[i].dir[0] += -en[i].dir[0] * CP_System_GetDt() * (check == 0 || check == 2);
    en[i].dir[1] += -en[i].dir[1] * CP_System_GetDt() * (check == 0 || check == 1);
  }
  return 0;
}

int moveBosses(enemy* bosses)
{
  for (int i = 0; i < MAX_BOSSESS; i++)
  {
    if (bosses[i].active == 0)
      continue;
    if (bosses[i].dir[0] <= 10 && bosses[i].dir[1] <= 10)
    {
      bosses[i].dir[0] = CP_Random_RangeFloat(-100, 100);
      if (bosses[i].x + bosses[i].dir[0] > CP_System_GetWindowWidth() / 2.0 || bosses[i].x + bosses[i].dir[0] < -CP_System_GetWindowWidth() / 2.0f)
        bosses[i].dir[0] = -bosses[i].dir[0];
      bosses[i].dir[1] = CP_Random_RangeFloat(-100, 100);
      if (bosses[i].y + bosses[i].dir[1] > CP_System_GetWindowHeight() / 2.0 || bosses[i].y + bosses[i].dir[1] < -CP_System_GetWindowHeight() / 2.0f)
        bosses[i].dir[1] = -bosses[i].dir[1];
    }
    bosses[i].x += bosses[i].dir[0] * CP_System_GetDt();
    bosses[i].y += bosses[i].dir[1] * CP_System_GetDt();
    bosses[i].dir[0] += -bosses[i].dir[0] * CP_System_GetDt();
    bosses[i].dir[1] += -bosses[i].dir[1] * CP_System_GetDt();
  }
  return 0;
}

int moveBullets(bullet* bullets, enemy* en, enemy* bosses, player *pl, item *items, building buildings[])
{

  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active)
    {
      int check = checkAgainstBuilding(buildings, 2, bullets[i] );
      bullets[i].active = 1 * (check == 0);
      bullets[i].x += (bullets[i].dir[0] * bullets[i].speed);
      bullets[i].y += (bullets[i].dir[1] * bullets[i].speed);
      bullets[i].life -= CP_System_GetDt();
      if (bullets[i].life <= 0)
      {
        bullets[i].active = 0;
        bullets[i].users = 0;
        bullets[i].speed = BULLET_SPEED;
      }
      float distance = sqrtf((bullets[i].x - (*pl).x) * (bullets[i].x - (*pl).x) + (bullets[i].y - (*pl).y) * (bullets[i].y - (*pl).y));
      if (distance < (*pl).playerRadius && bullets[i].users == 0)
      {
        if ((*pl).powerUp == 5)
          continue;
        (*pl).health -= bullets[i].pwr;
        bullets[i].active = 0;
        bullets[i].speed = BULLET_SPEED;
      }
      else if (bullets[i].users == 1)
      {
        for (int k = 0; k < MAX_ENEMIES; k++)
        {
          if (en[k].active == 0)
            continue;
          float distance2 = sqrtf((bullets[i].x - en[k].x) * (bullets[i].x - en[k].x) + (bullets[i].y - en[k].y) * (bullets[i].y - en[k].y));
          if (distance2 < en[k].radius / 2.0f)
          {
            en[k].health -= bullets[i].pwr;
            bullets[i].active = 0;
            bullets[i].speed = BULLET_SPEED;
            if (en[k].health <= 0)
            {
              (*pl).kills++;
              if (CP_Random_RangeInt(0, 100) < 75)
              {
                float temploc[2] = { en[k].x, en[k].y };
                dropItem(temploc, items);
              }
              en[k].active = 0;

            }
          }
        }
        for (int k = 0; k < MAX_BOSSESS; k++)
        {
          if (bosses[k].active == 0)
            continue;
          float distance2 = sqrtf((bullets[i].x - bosses[k].x) * (bullets[i].x - bosses[k].x) + (bullets[i].y - bosses[k].y) * (bullets[i].y - bosses[k].y));
          if (distance2 < bosses[k].radius / 2.0f)
          {
            bosses[k].health -= bullets[i].pwr;
            bullets[i].active = 0;
            bullets[i].speed = BULLET_SPEED;
            if (bosses[k].health <= 0)
            {
              (*pl).kills++;
              if (CP_Random_RangeInt(0, 100) < 75)
              {
                float temploc[2] = { bosses[k].x, bosses[k].y };
                dropItem(temploc, items);
              }
              bosses[k].active = 0;

            }
          }
        }
      }
    }
  }
  return 0;
}
