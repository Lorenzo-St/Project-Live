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

static int buildingNumb = 0;
static building* buildings;

int writeBuilding(building b, FILE* a);

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
  w = CP_Random_RangeFloat(200, 500);
  h = w;

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

void appendWorldFile(void) 
{
  FILE* worlds;
  FILE* worldsOld;
  errno_t e = 0;
  e = fopen_s(&worlds, "./Managed/Worlds.dat", "rb");
  if (e != 0)
    return;
  e = fopen_s(&worldsOld, "./Managed/WorldsOLD.dat", "wb");
  if (e != 0)
    return;
  char buffer;
  while (fread(&buffer, 1, 1, worlds) != 0) 
  {
    fwrite(&buffer, 1, 1, worldsOld);
  }
  fclose(worlds);
  fclose(worldsOld);
  e = fopen_s(&worlds, "./Managed/Worlds.dat", "wb");
  if (e != 0)
    return;
  e = fopen_s(&worldsOld, "./Managed/WorldsOLD.dat", "rb");
  if (e != 0)
    return;
  int count = 0;
  e = fscanf_s(worldsOld, "%i", &count);
  count += 1;
  fprintf(worlds, "%i\n", count);
  fprintf(worlds, "%s", getWorldName());
  while (fread(&buffer, 1, 1, worldsOld) != 0)
  {
    fwrite(&buffer, 1, 1, worlds);
  }
  fclose(worlds);
  fclose(worldsOld);
}

int generateWorld(void)
{
  time_t currTime;
  time(&currTime);
  FILE* fp;
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "./Managed/world%i.dat", (int)currTime);

  errno_t erro = 0;
  erro = fopen_s(&fp, buffer, "wb");
  if (erro != 0)
    return erro;
  buildingNumb = CP_Random_RangeInt(0, 255);
  fwrite(&buildingNumb, sizeof(int), 1, fp);
  buildings = malloc(sizeof(building) * buildingNumb);
  snprintf(buffer, sizeof(buffer), "world%i.dat", (int)currTime);
  char* worldName = malloc(sizeof(char) * (strlen(buffer) + 1));
  if (worldName == NULL)
    return 1;
  snprintf(worldName, strlen(buffer) + 1, "world%i.dat", (int)currTime);
  setWorldName(worldName);
  setGenerated(true);
  if (buildings == NULL)
  {
    fclose(fp);
    return 1;
  }
  for (int i = 0; i < buildingNumb; i++)
  {
    generateBuildings(buildings + i, i);
    writeBuilding(*(buildings + i), fp);
  }
  fclose(fp);
  appendWorldFile();
  return 0;
}

int writeBuilding(building b, FILE* a)
{
  if (a == NULL)
    return 1;

  char buffer[4 * sizeof(float) + sizeof(char)] = { 0 };
  char offset = 0;
  char type = (char)b.hasInterior + (b.type << 1) + (b.doorSide << 2);
  memmove(buffer, &type, sizeof(type));
  offset += sizeof((type));
  memmove(buffer + offset, &(b.x), sizeof((b.x)));
  offset += sizeof((b.x));
  memmove(buffer + offset, &(b.y), sizeof((b.y)));
  offset += sizeof((b.y));
  memmove(buffer + offset, &(b.w), sizeof((b.w)));
  offset += sizeof((b.w));
  memmove(buffer + offset, &(b.h), sizeof((b.h)));
  offset += sizeof((b.h));
  fwrite(buffer, 1, 4 * sizeof(float) + sizeof(char), a);

  return 0;
}

void readBuilding(building* b, FILE* a)
{
  char buffer[4 * sizeof(float) + sizeof(char)] = { 0 };
  fread(buffer, 1, sizeof(buffer), a);
  int offset = 0;
  char type = buffer[offset++];
  b->hasInterior = type & 0b001;
  b->type = type & 0b010;
  type >>= 2;
  b->doorSide = type;
  memmove(&b->x, buffer + offset, sizeof(b->x));
  offset += sizeof(b->x);
  memmove(&b->y, buffer + offset, sizeof(b->y));
  offset += sizeof(b->y);
  memmove(&b->w, buffer + offset, sizeof(b->w));
  offset += sizeof(b->w);
  memmove(&b->h, buffer + offset, sizeof(b->h));
  offset += sizeof(b->h);
}

int loadWorld(char* worldName) 
{
  char* pathStart = "./Managed/";
  FILE* fp = NULL;
  int pathLen = (int)(strlen(worldName) + strlen(pathStart) + 1);
  char* path = malloc(pathLen);
  if (path == NULL)
    return 1;
  strcpy_s(path, pathLen, pathStart);
  strcat_s(path, pathLen, worldName);
  errno_t erro = 0;
  erro = fopen_s(&fp, path, "rb");
  if(erro != 0)
    return erro;
  free(path);
  fread_s(&buildingNumb, sizeof(int), sizeof(int), 1, fp);
  buildings = malloc(sizeof(building) * buildingNumb);
  if (buildings == NULL)
  {
    fclose(fp);
    return 1;
  }
  for (int i = 0; i < buildingNumb; i++) 
  {
    readBuilding(buildings + i, fp);
  }
  return 0;
}