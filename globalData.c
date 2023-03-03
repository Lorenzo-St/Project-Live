#include "structs.h"
#include "worldSystems.h"
#define MAX_WORLD_NAME_LENGTH 30

static float time = 0;
static int highestBulletCount = 0;
static bool isPaused = false;
static bool inGame = false;
static bool inOptions = false;
static bool generated = false;
static const char* const version = "0.5.01";
static char* worldName = NULL;
static char  newWorldName[MAX_WORLD_NAME_LENGTH] = { 0 };


int grabBuildingNumb(void) 
{
  return getNumOfBuildings();
}

char* getWorldName(void) 
{
  return worldName;
}

char* getNewName(void) 
{
  return newWorldName;
}


const char* getVersion(void) 
{
  return version;
}

bool getOptions(void) 
{
  return inOptions;
}

bool getGame(void) 
{
  return inGame;
}

bool getPause(void) 
{
  return isPaused;
}

bool getGenerated(void) 
{
  return generated;
}

void setGame(bool a) 
{
  inGame = a;
}

void setPause(bool a) 
{
  isPaused = a;
}

void setWorldName(char* name) 
{
  worldName = name;
}

void setOptions(bool a)
{
  inOptions = a;
}

void setGenerated(bool a) 
{
  generated = a;
}

float getTime(void) 
{
  return time;
}
void setTime(float a) 
{
  time = a;
}
void addTime(float a) 
{
  time += a;
}

int getBulletCount(void) 
{
  return highestBulletCount;
}

void setBulletCount(int a) 
{
  highestBulletCount = a;
}