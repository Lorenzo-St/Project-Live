#include "worldInit.h"
#include "structs.h"
#include "cprocessing.h"

int initStrings(string* pickupText)
{
  for (int i = 0; i < MAX_TEXT; i++)
  {
    pickupText[i].active = 0;
    pickupText[i].x, pickupText[i].y = 0;
    pickupText[i].life = 1.5f;
  }
  return 0;
}

int initBosses(enemy *bosses)
{
  for (int i = 0; i < MAX_BOSSESS; i++)
  {
    float Ex = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
    float Ey = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
    bosses[i] = (enemy){ 0, Ex, Ey };
    bosses[i].speed = 50;
    bosses[i].cooldown = .2f;
    bosses[i].type = 5;
    bosses[i].radius = 100;
    bosses[i].health = 400;
    bosses[i].pattern = CP_Random_RangeInt(0, 3);
  }

  return 0;
}

void initAudio(CP_Sound* bulletSounds)
{
  bulletSounds[0] = CP_Sound_Load("./Assets/pew.mp3");
}

int initEnemies(enemy* en)
{
  for (int i = 0; i < MAX_ENEMIES; i++)
  {

    float Ex = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
    float Ey = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
    en[i] = (enemy){ 0, Ex, Ey };
    en[i].speed = 50;
    en[i].cooldown = 1.5f;
    en[i].type = CP_Random_RangeInt(0, 1);
    en[i].radius = 25;
    en[i].health = 60;

    if (i > 3)
    {
      en[i].active = 0;
    }
    else
    {
      en[i].active = 1;
      en[i].type = 0;
    }
  }
  return 0;
}

int initBullets(bullet* bullets)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    bullets[i] = (bullet){ 0,0,0,0,0,BULLET_SPEED,10 };
  }
  return 0;
}

int initDrops(item* items)
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    items[i] = (item){ 0 };
    items[i].radius = 30.0f;
  }
  return 0;
}
