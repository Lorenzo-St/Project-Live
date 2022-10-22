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