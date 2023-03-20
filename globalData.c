#include "structs.h"
#include "worldSystems.h"
#include "globalImages.h"
#include "profileData.h"
#define MAX_WORLD_NAME_LENGTH 30

static float time = 0;
static int highestBulletCount = 0;
static bool isPaused = false;
static bool inGame = false;
static bool inOptions = false;
static bool generated = false;
static const char* const version = "0.5.7";
static char* worldName = NULL;
static char  newWorldName[MAX_WORLD_NAME_LENGTH] = { 0 };
static ColorMode colorMode = LightMode;

ColorMode getColorMode(void) 
{
  return colorMode;
}

void setColorMode(ColorMode c) 
{
  colorMode = c;
  SetColorMode(c);
  updateColorMode();
}

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