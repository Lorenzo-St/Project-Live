#include "worldInit.h"
#include "structs.h"
#include "cprocessing.h"
#include "globalData.h"
#include "checkStuff.h"


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
