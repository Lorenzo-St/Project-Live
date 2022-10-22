#pragma once


/* |----------------------- Defines ---------------------------| */
#define MAX_ENEMIES 100
#define MAX_BULLETS 500
#define MAX_DROPS 10
#define KEY_COUNT 6
#define SPREAD_COUNT 10
#define BULLET_SPEED 10
#define MAX_BOSSESS 10
#define BASE_PATTERN 10
#define MAX_TEXT 10
#define AUDIOS 2


/* |----------------------- Structs ---------------------------| */

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

typedef struct slider{

  char title[100];
  int selected;
  float x;
  float y;
  float length;
  float value; /* Between 0 and 1 */
}slider;

typedef struct button
{
  int selected;
  char words[100];
  float x;
  float y;
  float width;
  float height;


}button;

typedef struct bullet
{
  int active;
  int users;
  int pwr;
  float x;
  float y;
  float dir[2];
  float speed;
  float life;
  float radius;


}bullet;

typedef struct item
{
  int active;
  float x;
  float y;
  int type;/*0 is health, 1 is weapon, 2 powerup*/
  int containes; /*must be non zero, if its a health pickup then containes is how much health it containes, if its a weapon then containes is what weapon it is*/
  float life;
  float radius;
}item;

typedef struct enemy
{
  int active;
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
  int active;
  float x, y;
  float life;
  char buffer[100];
}string;