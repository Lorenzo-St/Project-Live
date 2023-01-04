#include "structs.h"

float time = 0;
int highestBulletCount = 0;
bool isPaused = false;
bool inGame = false;
bool inOptions = false;
const char* const version = "0.4.10";

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

void setGame(bool a) 
{
  inGame = a;
}

void setPause(bool a) 
{
  isPaused = a;
}

void setOptions(bool a)
{
  inOptions = a;
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