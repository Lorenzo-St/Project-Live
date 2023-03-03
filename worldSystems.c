/* Headers */
#include "randFunctions.h"
#include "structs.h"
#include "cprocessing.h"
#include "globalData.h"

/* Libraries */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>

static int buildingNumb = 0;
static building* buildings;

int getNumOfBuildings(void) 
{
  return buildingNumb;
}

building* returnBuildings(void) 
{
  return buildings;
}

void generateBuildings(building* b, int index)
{
  float x;
  float y;
  float w;
  float h;
  x = CP_Random_RangeFloat(-10000, 10000);
  y = CP_Random_RangeFloat(-10000, 10000);
  w = CP_Random_RangeFloat(300, 2000);
  h = CP_Random_RangeFloat(300, 2000);;
  x = (float)(int)x;
  y = (float)(int)y;
  w = (float)(int)w;
  h = (float)(int)h;

  if (index != 0) 
  {
    for (int i = index; i > 0; i--) 
    {
      building* lastOne = b - i;
      float distance = sqrtf((lastOne->x - x) * (lastOne->x - x) + (lastOne->y - y) * (lastOne->y - y));
      float minDist = w / 2 + lastOne->w / 2;
      while (distance < minDist)
      {
        x = CP_Random_RangeFloat(-10000, 10000);
        y = CP_Random_RangeFloat(-10000, 10000);
        distance = sqrtf((lastOne->x - x) * (lastOne->x - x) + (lastOne->y - y) * (lastOne->y - y));
      }
    }
  }
  b->x = x;
  b->y = y;
  b->w = w;
  b->h = h;
  b->hasInterior = CP_Random_RangeInt(0, 1);
  if (b->hasInterior) 
  {
    b->doorSide = (unsigned char)CP_Random_RangeInt(0, 3);
    b->type = 0;
  }
}

int generateWorld(void)
{
  time_t currTime;
  time(&currTime);


  buildingNumb = CP_Random_RangeInt(0, 255);
  buildings = malloc(sizeof(building) * buildingNumb);

  if (buildings == NULL)
  {
    return 1;
  }
  for (int i = 0; i < buildingNumb; i++)
  {
    generateBuildings(buildings + i, i);
  }
  return 0;
}
