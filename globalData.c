#include "structs.h"
#include "worldSystems.h"

float time = 0;
int highestBulletCount = 0;
bool isPaused = false;
bool inGame = false;
bool inOptions = false;
bool generated = false;
const char* const version = "0.4.21";
char* worldName = NULL;



int grabBuildingNumb(void) 
{
  return getNumOfBuildings();
}

char* getWorldName(void) 
{
  return worldName;
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