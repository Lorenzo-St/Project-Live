#include "worldInit.h"
#include "structs.h"
#include "cprocessing.h"
#include "globalData.h"
#include "checkStuff.h"

int initBuildings(building* buildings)
{
  for (int i = 0; i < NUMBER_OF_BUILDINGS; i++) 
  {
    buildings[i] = (building){ CP_Random_RangeFloat(-10000, 10000),CP_Random_RangeFloat(-1000, 1000),CP_Random_RangeFloat(100, 2000), CP_Random_RangeFloat(100, 2000),CP_Random_RangeInt(0, 1), CP_Random_RangeInt(0, 3) };
  }
  return 0;
}

int initPlayer(player *pl, float* multiplier, float* addTimer)
{
  pl->powerUp = 0;
  pl->powerUpTimer = 0.0f;
  pl->x, pl->y = 0;
  pl->move_speed = 100;
  pl->cooldown = 1.0f;
  *addTimer = 5.0f;
  pl->kills = 0;
  pl->weapon = 0;
  pl->health = 200;
  pl->playerRadius = 20;
  *multiplier = 1;
  setTime(0.0f);
  return 0;
}

int initStrings(notiString *pickupText)
{
  for (int i = 0; i < MAX_TEXT; i++)
  {
    pickupText[i].active = 0;
    pickupText[i].x, pickupText[i].y = 0;
    pickupText[i].life = 1.5f;
  }
  return 0;
}

void initAudio(CP_Sound* bulletSounds)
{
  bulletSounds[0] = CP_Sound_Load("./Assets/pew.mp3");
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
