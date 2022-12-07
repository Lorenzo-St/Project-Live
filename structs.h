#pragma once
#include "cprocessing.h"
/* |----------------------- Defines  ---------------------------| */
#define MAX_ENEMIES 100
#define MAX_BULLETS 500
#define MAX_DROPS 10
#define MAX_ITEMS 6
#define KEY_COUNT 9
#define SPREAD_COUNT 10
#define BULLET_SPEED 10
#define MAX_BOSSESS 10
#define BASE_PATTERN 10
#define MAX_TEXT 10
#define AUDIOS 2
#define NUMBER_OF_BUILDINGS 50
#define FALSE 0
#define false FALSE
#define TRUE 1
#define true TRUE
#define ITEMS_PER_ROW 10
#define WHEEL_SIZE 6
#define SCREEN_WIDTH  CP_System_GetWindowWidth()
#define SCREEN_HEIGHT CP_System_GetWindowHeight()

/* |----------------------- Colors   ---------------------------| */
#define INVENTORY CP_Color_CreateHex(0X505050BA)
#define WHEEL_BG  CP_Color_CreateHex(0xC7C5FC7C)
#define WHEEL_FG  CP_Color_CreateHex(0xe0e7ee7C)
#define BLACK     CP_Color_CreateHex(0x000000FF)
#define WHITE     CP_Color_CreateHex(0xFFFFFFFF)

/* |-----------------------  types   ---------------------------| */

typedef unsigned int bool;

typedef struct player
{
  int health;
  int weapon;  /*0 signifies a slow pistol, 1 is a single barrel machine gun, 2 is a sniper riffle, 3 is a shotgun, 4 is semi-Auto Pistol*/
  float playerRadius;
  float cooldown;
  float move_speed;
  float x, y;
  float direction[2];
  float velocity[2];
  float rot;
  int kills;
  int powerUp; /* 0: no powerup, 1: insta kill, 2: RAPID FIRE, 3: FULL HEAL, 4: SCREEN WIPE, 5: INVINCIBILITY, 6: TIME WARP (WIP)  */
  float powerUpTimer;
}player;

typedef struct Camera 
{
  float x, y;
}camera;

typedef struct slider{

  char title[100];
  bool selected;
  float x;
  float y;
  float length;
  float value; /* Between 0 and 1 */
}slider;

typedef struct building 
{
  float x, y;
  float xLen, yLen;
  bool hasInterior;
  int doorSide;
}building;

typedef struct button
{
  bool selected;
  char words[100];
  float x;
  float y;
  float width;
  float height;


}button;

typedef struct bullet
{
  bool active;
  bool users;
  int pwr;
  float x;
  float y;
  float dir[2];
  float speed;
  float life;
  float radius;


}bullet;

typedef struct invItem 
{
  char* name;
  char itemId;
  unsigned char type;/* 0 == weapon, 1 == material, 2+ == ??? */
  bool stackable;
  int count;
  float durability;
  struct invItem* next;
}InvItem;

typedef struct item
{
  bool active;
  float x;
  float y;
  int type;/*0 is health, 1 is weapon, 2 powerup*/
  int containes; /*must be non zero, if its a health pickup then containes is how much health it containes, if its a weapon then containes is what weapon it is*/
  float life;
  float radius;
}item;

typedef struct enemy
{
  bool active;
  float x;
  float y;
  float cooldown;
  float dir[2];
  float speed;
  float radius;
  int health;
  int type; /*0 is basic, 1 is rapid fire*/
  /* Only Used with boses */
  int pattern;

}enemy;

typedef struct string
{
  bool active;
  float x, y;
  float life;
  char buffer[100];
}string;

/* |----------------------- Gloabals ---------------------------| */

