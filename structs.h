#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <stdbool.h>
static int playerKeys[] = { KEY_W, KEY_A, KEY_S, KEY_D, KEY_LEFT_SHIFT, KEY_SPACE, KEY_E, KEY_I, KEY_Q, KEY_R, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LEFT_CONTROL };
int getMaxEnemies(void);

/* |----------------------- Defines  ---------------------------| */
#define MAX_BULLETS 500
#define MAX_ENEMIES getMaxEnemies()
#define MAX_DROPS 10
#define MAX_ITEMS 6
#define KEY_COUNT _countof(playerKeys)
#define SPREAD_COUNT 10
#define BULLET_SPEED 10
#define MAX_BOSSESS 10
#define BASE_PATTERN 10
#define MAX_TEXT 10
#define AUDIOS 2
#define NUMBER_OF_BUILDINGS 50
#define ITEMS_PER_ROW 10
#define WHEEL_SIZE 6
#define SCREEN_WIDTH  CP_System_GetWindowWidth()
#define SCREEN_HEIGHT CP_System_GetWindowHeight()
#define WEAPON_IMAGES 4
#define AMMO_TYPES 3
#define ENEMY_TYPE 2 /* starts at 0*/
#define MAX_OBJECTIVES 1
#define WINDOW_SIZE 1920, 1080
#define MAX_INV_ITEMS 100

 /* Crafting numbers */
#define SMALL_T_MED_COMB 25
#define MATERIALS 3
#define SMALL_AMMO_COST 5
#define MEDIU_AMMO_COST 10
#define LARGE_AMMO_COST 25
#define BASE_KIT_COST 4
#define HEALTH_PER_KIT 100
#define MAX_HEALTH 250
#define MISC_GRAPHICS 1

/* |----------------------- Colors   ---------------------------| */
#define INVENTORY CP_Color_CreateHex(0X505050BA)
#define INV_ITEM  CP_Color_CreateHex(0X9FA9A3FF)
#define WHEEL_BG  CP_Color_CreateHex(0xC7C5FC7C)
#define WHEEL_FG  CP_Color_CreateHex(0xe0e7ee7C)
#define BLACK     CP_Color_CreateHex(0x000000FF)
#define WHITE     CP_Color_CreateHex(0xFFFFFFFF)
#define GRAY      CP_Color_CreateHex(0x3A3A3AFF)
#define GRAY_BUT  CP_Color_CreateHex(0x5A5A5AFF)
#define BAD_BUTT  CP_Color_CreateHex(0xF03636FF)
#define RED       CP_Color_CreateHex(0xFA1212FF)

/* |-----------------------  types   ---------------------------| */

typedef enum ColorMode 
{
  LightMode,
  DarkMode,

}ColorMode;


typedef struct objective 
{
  char* title;
}objective;

typedef struct button
{
  bool selected;
  char words[100];
  float x;
  float y;
  float width;
  float height;

}button;

typedef struct scorllableMenu
{
  int scrollPosition;
}scrolllable;

typedef struct weaponData
{
  int type;  /* */
  int damage;
  int ammoReserves;
  int ammo;
  int ammoType;
  int pattern;
  float bulletSpeed;
  float attackSpeed;
  float reloadTime;
  float reloadClock;
  
}weaponData;





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
  bool hasInterior;
  char type;
  char doorSide;
  float x, y;
  float w, h;

}building;


typedef struct bullet
{
  bool users;
  int pwr;
  float x;
  float y;
  float dir[2];
  float speed;
  float life;
  float radius;
  struct bullet* next;

}bullet;

typedef struct invItem 
{
  const char* name;
  char itemId;
  char weaponId;
  char subID;
  unsigned char type;/* 0 == weapon, 1 == light and medium ammo, 2 == materials, 3 == health kits, 4 == */
  bool stackable;
  int count;
  float durability;
}InvItem;

typedef struct item
{
  bool active;
  float x;
  float y;
  char id;
  float life;
  float radius;
}item;

typedef struct enemy
{
  CP_Vector pos;
  CP_Vector dir;
  weaponData* weapon;
  float speed;
  float radius;
  int OOBCOunt;
  int health;
  int MaxHealth;
  int type; /*0 is basic, 1 is rapid fire, 2 is boss*/
  int lvl;
  bool alive;
}enemy;

typedef struct notiString
{
  bool active;
  float x, y;
  float life;
  char buffer[100];
}notiString;

typedef struct wheelAmmo
{
  int active[WHEEL_SIZE];
  int reserves[WHEEL_SIZE];
  int lightStorage;
  int mediumStorage;
  int heavyStorage;
}wheelAmmo;


/* |----------------------- Gloabals ---------------------------| */


