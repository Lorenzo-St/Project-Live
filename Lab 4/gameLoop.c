//---------------------------------------------------------
// file:	gamestate_template.c
// author:	Lorenzo St. Luce
// email:	 lorenzo.stluce@digipen.edu
//
// brief:	template file for holding gamestate functions
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

/* |---------------------- Includes ---------------------------| */
#include "cprocessing.h"
#include "endScreen.h"
#include "gameLoop.h"
#include "structs.h"
#include "globalData.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
int playerFire(void);


/* |---------------------- Game Stuff --------------------| */

/* Gameworld and enemy stats and storage */
int bossesEnabled = 0;
int screen = 0;
int isPaused = 0;
int kills = 0;
int powerUp = 0; /* 0: no powerup, 1: insta kill, 2: RAPID FIRE, 3: FULL HEAL, 4: SCREEN WIPE, 5: INVINCIBILITY, 6: TIME WARP (WIP)  */
float powerUpTimer = 0;
float addTimer = 5.0f;
float multiplier = 1;
enemy en[MAX_ENEMIES] = { 0 };
enemy bosses[MAX_BOSSESS] = { 0 };
bullet bullets[MAX_BULLETS] = { 0 };
item items[MAX_DROPS] = { 0 };
string pickupText[MAX_TEXT] = { 0 };
CP_Sound bulletSounds[AUDIOS] = { 0 };

/*Player statsand data storage items */

int health = 200;
int weapon = 0;  /*0 signifies a slow pistol, 1 is a single barrel machine gun, 2 is a sniper riffle, 3 is a shotgun, 4 is semi-Auto Pistol*/
int playerKeys[KEY_COUNT] = { 87, 65, 83, 68, 340, 32 };
float playerRadius = 20;
float cooldown = 1.5;
float move_speed = 100;
float x, y = 0;
float direction[2] = { 0 };
float velocity[2] = { 0 };
float rot = 0;


int initPlayer(void)
{
  powerUp = 0;
  powerUpTimer = 0.0f;
  x, y = 0;
  move_speed = 100;
  cooldown = 1.0f;
  addTimer = 5.0f;
  kills = 0;
  weapon = 0;
  health = 200;
  multiplier = 1;
  setTime(0.0f);
  return 0;
}

int initStrings(void) 
{
  for (int i = 0; i < MAX_TEXT; i++) 
  {
    pickupText[i].active = 0;
    pickupText[i].x, pickupText[i].y = 0;
    pickupText[i].life = 1.5f;
  }
  return 0;
}

int initBosses(void) 
{
  for (int i = 0; i < MAX_BOSSESS; i++) 
  {
    float Ex = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
    float Ey = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
    bosses[i] = (enemy){ 0, Ex, Ey };
    bosses[i].speed = 50;
    bosses[i].cooldown = .2f;
    bosses[i].type = 5;
    bosses[i].radius = 100;
    bosses[i].health = 400;
    bosses[i].pattern = CP_Random_RangeInt(0, 3);
  }

  return 0;
}

void initAudio(void) 
{
  bulletSounds[0] = CP_Sound_Load("./Assets/pew.mp3");
}

int initEnemies(void)
{
  for (int i = 0; i < MAX_ENEMIES; i++)
  {

    float Ex = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
    float Ey = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
    en[i] = (enemy){ 0, Ex, Ey };
    en[i].speed = 50;
    en[i].cooldown = 1.5f;
    en[i].type = CP_Random_RangeInt(0, 1);
    en[i].radius = 25;
    en[i].health = 60;

    if (i > 3)
    {
      en[i].active = 0;
    }
    else
    {
      en[i].active = 1;
      en[i].type = 0;
    }
  }
  return 0;
}

int initBullets(void)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    bullets[i] = (bullet){ 0,0,0,0,0,BULLET_SPEED,10 };
  }
  return 0;
}

int initDrops(void)
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    items[i] = (item){ 0 };
    items[i].radius = 30.0f;
  }
  return 0;
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gameLoopInit(void)
{
  CP_Settings_RectMode(CP_POSITION_CENTER);
  CP_Settings_EllipseMode(CP_POSITION_CENTER);
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  bossesEnabled = 0;
  initAudio();
  initPlayer();
  initEnemies();
  initBullets();
  initDrops();
  initBosses();

  // initialize variables and CProcessing settings for this gamestate
}


/* DRAW Stuff */

void drawWeapon(void)
{
  float screen_center = CP_System_GetWindowWidth() / 2.0f;
  CP_Settings_Fill(CP_Color_Create(220, 220, 220, 100));
  CP_Graphics_DrawRectAdvanced(screen_center, 50, 500, 200, 0, 45);
  switch (weapon)
  {
  case 0:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center, 30, 100, 20, 0, 10);

    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center + 30, 55, 30, 55, -15, 10);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("Pistol", screen_center, 120);
    break;
  case 4:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center, 30, 100, 20, 0, 10);
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center + 30, 55, 30, 55, -15, 10);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("Pistol (Semi-Auto)", screen_center, 120);
    break;
  case 1:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center - 20, 40, 140, 20, 0, 10);
    CP_Graphics_DrawRectAdvanced(screen_center + 60, 50, 70, 20, 0, 10);
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center + 30, 60, 25, 50, -15, 10);
    CP_Graphics_DrawRectAdvanced(screen_center - 10, 60, 25, 30, -10, 5);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("Auto Rifle", screen_center, 120);
    break;
  case 2:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center - 30, 40, 250, 20, 0, 10);
    CP_Graphics_DrawRectAdvanced(screen_center + 70, 50, 90, 20, 0, 10);
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center - 25, 55, 60, 20, 0, 5);
    CP_Graphics_DrawRectAdvanced(screen_center, 25, 60, 15, 0, 5);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("1.5\' Long boii Hours", screen_center, 120);
    break;
  case 3:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center - 20, 40, 140, 20, 0, 10);
    CP_Graphics_DrawRectAdvanced(screen_center + 60, 50, 70, 20, 0, 10);
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center + 30, 60, 25, 30, -25, 10);
    CP_Graphics_DrawRectAdvanced(screen_center - 25, 55, 60, 20, 0, 5);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("Shoty 5k", screen_center, 120);
    break;
  }
  char buffer[100];
  if (powerUpTimer > 0) 
  {
    switch (powerUp) 
    {
    case 1:
      snprintf(buffer, sizeof buffer, "Instal Kill: %f", powerUpTimer);
      break;
    case 2:
      snprintf(buffer, sizeof buffer, "Rapid Fire: %f", powerUpTimer);
      break;
    case 5:
      snprintf(buffer, sizeof buffer, "Invincibility: %f", powerUpTimer);
      break;
    }
    CP_Font_DrawText(buffer, screen_center, 300);
  }
}

void drawPlayer(void)
{
  CP_Settings_StrokeWeight(1.5f);
  CP_Settings_Fill(CP_Color_CreateHex(0x99A3A4));
  CP_Graphics_DrawCircle(x + (CP_System_GetWindowWidth() / 2.0f), -y + (CP_System_GetWindowHeight() / 2.0f), playerRadius * 2);
  CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
  CP_Graphics_DrawRectAdvanced((x + ((direction[0] * playerRadius) / sqrtf(direction[0] * direction[0] + direction[1] * direction[1]))) + (CP_System_GetWindowWidth() / 2.0f), (-y + ((direction[1] * playerRadius) / sqrtf(direction[0] * direction[0] + direction[1] * direction[1]))) + (CP_System_GetWindowHeight() / 2.0f), playerRadius / 1.25f, 10, rot, 0.0f);
}

void drawBullets(void)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active == 0)
      continue;
    if (bullets[i].users == 0)
      CP_Settings_Fill(CP_Color_Create(10, 10, 25, 255));
    else if (bullets[i].users == 1)
      CP_Settings_Fill(CP_Color_Create(201, 254, 255, 255));

    CP_Settings_StrokeWeight(0.0f);
    CP_Graphics_DrawCircle(bullets[i].x + (CP_System_GetWindowWidth() / 2.0f), -bullets[i].y + (CP_System_GetWindowHeight() / 2.0f), bullets[i].radius);
  }
}

void drawEnemies(void)
{
  CP_Settings_Fill(CP_Color_Create(226, 225, 76, 255));
  for (int i = 0; i < MAX_ENEMIES; i++)
  {
    if (en[i].active == 0)
      continue;
    CP_Graphics_DrawCircle(en[i].x + (CP_System_GetWindowWidth() / 2.0f), -en[i].y + (CP_System_GetWindowHeight() / 2.0f), en[i].radius);
  }
}

void drawBosses(void)
{
  CP_Settings_Fill(CP_Color_Create(50, 100, 230, 255));
  for (int i = 0; i < MAX_BOSSESS; i++)
  {
    if (bosses[i].active == 0)
      continue;
    CP_Graphics_DrawCircle(bosses[i].x + (CP_System_GetWindowWidth() / 2.0f), -bosses[i].y + (CP_System_GetWindowHeight() / 2.0f), bosses[i].radius);
  }
}

void drawPickupText(void) 
{
  CP_Settings_TextSize(30);
  for (int i = 0; i < MAX_TEXT; i++) 
  {
    if (pickupText[i].active == 0)
      continue;
    if (pickupText[i].life <= 0)
    {
      pickupText[i].active = 0;
      continue;
    }
    CP_Settings_Fill(CP_Color_Create(100, 100, 100, 125));
    CP_Font_DrawText(pickupText[i].buffer, pickupText[i].x + (CP_System_GetWindowWidth() / 2.0f), -pickupText[i].y + (CP_System_GetWindowHeight() / 2.0f));
    pickupText[i].y++;
    pickupText[i].life -= CP_System_GetDt();
  }
}

void drawItems(void)
{
  CP_Settings_StrokeWeight(0.0f);
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
      continue;

    switch (items[i].type)
    {
    case 0:
      CP_Settings_Fill(CP_Color_Create(19, 136, 8, 255));
      CP_Graphics_DrawRectAdvanced(items[i].x + (CP_System_GetWindowWidth() / 2.0f), -items[i].y + (CP_System_GetWindowHeight() / 2.0f), items[i].radius, items[i].radius, 45.0f, 0.0f);
      CP_Settings_TextSize(10);
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Font_DrawText("Health", items[i].x + (CP_System_GetWindowWidth() / 2.0f), -items[i].y + (CP_System_GetWindowHeight() / 2.0f));
      break;
    case 1:
      CP_Settings_Fill(CP_Color_Create(201, 242, 201, 255));
      CP_Graphics_DrawRectAdvanced(items[i].x + (CP_System_GetWindowWidth() / 2.0f), -items[i].y + (CP_System_GetWindowHeight() / 2.0f), items[i].radius, items[i].radius, 0, 5);
      CP_Settings_TextSize(10);
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Font_DrawText("Weapon", items[i].x + (CP_System_GetWindowWidth() / 2.0f), -items[i].y + (CP_System_GetWindowHeight() / 2.0f));
      break;
    case 2:
      CP_Settings_Fill(CP_Color_Create(250, 100, 255, 255));
      CP_Graphics_DrawRectAdvanced(items[i].x + (CP_System_GetWindowWidth() / 2.0f), -items[i].y + (CP_System_GetWindowHeight() / 2.0f), items[i].radius, items[i].radius, 0, 5);
      CP_Settings_TextSize(10);
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Font_DrawText("POWERUP", items[i].x + (CP_System_GetWindowWidth() / 2.0f), -items[i].y + (CP_System_GetWindowHeight() / 2.0f));
      break;
    }
  }
}


/* ADD Stuff*/

int addPickup(item a) 
{
  for (int i = 0; i < MAX_TEXT; i++) 
  {
    if (pickupText[i].active)
      continue;

    pickupText[i].active = 1;
    pickupText[i].life = 1.5f;
    pickupText[i].x = a.x;
    pickupText[i].y = a.y;
    switch (a.type) 
    {
    case 0:
      snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Health Aquired");
      break;
    case 1:
      switch (a.containes)
      {
      case 1:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Picked up Autorifle");
        break;
      case 2:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Picked up Sniper");
        break;
      case 3:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Picked up Shotgun");
        break;
      case 4:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Picked up Semi-auto Pistol");
        break;
      }
      break;
    case 2:
      switch (a.containes) 
      {
      case 1:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "INSTA-KILL!!");
        break;
      case 2:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "RAPID FIRE!!");
        break;
      case 3:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "FULL HEAL!!");
        break;
      case 4:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "KA-BOOOM!!!!");
        break;
      case 5:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "INVINCIBILITY!!");
        break;
      }
      break;
    }
  }
  return 0;
}

void addEnemy(void)
{
  if (bossesEnabled == 0)
  {
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
      if (en[i].active == 1)
        continue;
      if (en[i].active == 0)
      {
        en[i].active = 1;
        en[i].x = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
        en[i].y = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
        en[i].speed = 50;
        en[i].cooldown = 1.5f;
        en[i].type = CP_Random_RangeInt(0, 1);
        en[i].health = 60;
        break;
      }
    }
  }
  else
  {
    int side = CP_Random_RangeInt(0, 100);
    if (side > 25)
    {
      for (int i = 0; i < MAX_ENEMIES; i++)
      {
        if (en[i].active == 1)
          continue;
        if (en[i].active == 0)
        {
          en[i].active = 1;
          en[i].x = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
          en[i].y = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
          en[i].speed = 50;
          en[i].cooldown = 1.5f;
          en[i].type = CP_Random_RangeInt(0, 1);
          en[i].health = 60;
          break;
        }
      }

    }
    else if (side <= 25)
    {
      for (int i = 0; i < MAX_BOSSESS; i++)
      {
        if (bosses[i].active == 1)
          continue;
        if (bosses[i].active == 0)
        {
          bosses[i].active = 1;
          bosses[i].x = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
          bosses[i].y = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
          bosses[i].speed = 25;
          bosses[i].cooldown = .2f;
          bosses[i].health = 300;
          break;
        }
      }
    }
  }

}

int dropItem(float loc[])
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
    {
      items[i].x = loc[0];
      items[i].y = loc[1];
      items[i].type = CP_Random_RangeInt(0, 2);
      switch (items[i].type)
      {
      case 0:
        items[i].containes = CP_Random_RangeInt(40, 90);
        break;
      case 1:
        items[i].containes = CP_Random_RangeInt(1, 4);
        break;
      case 2:
        items[i].containes = CP_Random_RangeInt(1, 5);
        break;
      }
      items[i].active = 1;
      items[i].life = 10;
      return 0;
    }
  }
  return 0;
}


/* Check Stuff */

int checkKeys(void)
{
  velocity[0] = 0.0f;
  velocity[1] = 0.0f;
  int errored = 0;
  float velo = (move_speed * CP_System_GetDt());
  if (CP_Input_MouseClicked())
  {
    if (weapon == 4)
      playerFire();
  }
  if (CP_Input_MouseDown(MOUSE_BUTTON_1))
  {
    if (cooldown <= 0 && weapon != 4)
    {
      playerFire();
      switch (weapon)
      {
      case 0:
        if (powerUp != 2)
          cooldown = .8f;
        else if (powerUp == 2)
          cooldown = .1f;
        break;
      case 1:
        if (powerUp != 2)
          cooldown = .1f;
        else if (powerUp == 2)
          cooldown = .0f;

        break;
      case 2:
        if (powerUp != 2)
          cooldown = 1.3f;
        else if (powerUp == 2)
          cooldown = .13f;

        break;
      case 3:
        if (powerUp != 2)
          cooldown = 1.5f;
        else if (powerUp == 2)
          cooldown = .15f;
        break;
      }
    }
  }
  for (int i = 0; i < KEY_COUNT; i++)
  {
    if (CP_Input_KeyTriggered(playerKeys[i]))
    {
      switch (playerKeys[i])
      {
      case 340:
        if (multiplier <= 1)
        {
          multiplier = 4;
        }
        break;
      case 32:
        if (weapon == 4)
          playerFire();
        break;
      }
    }
    if (CP_Input_KeyDown(playerKeys[i]))
    {
      switch (playerKeys[i])
      {
      case 87:
        velocity[1] += velo;
        break;
      case 65:
        velocity[0] += -velo;
        break;
      case 83:
        velocity[1] += -velo;
        break;
      case 68:
        velocity[0] += velo;
        break;
      case 256:
        if (isPaused)
          isPaused = 0;
        else
          isPaused = 1;
        break;
      case 32:
        if (weapon == 4)
          break;
        if (cooldown <= 0)
        {
          playerFire();
          switch (weapon)
          {
          case 0:
            if (powerUp != 2)
              cooldown = .8f;
            else if (powerUp == 2)
              cooldown = .1f;
            break;
          case 1:
            if (powerUp != 2)
              cooldown = .1f;
            else if (powerUp == 2)
              cooldown = .0f;

            break;
          case 2:
            if (powerUp != 2)
              cooldown = 1.3f;
            else if (powerUp == 2)
              cooldown = .13f;

            break;
          case 3:
            if (powerUp != 2)
              cooldown = 1.5f;
            else if (powerUp == 2)
              cooldown = .15f;
            break;
          }
        }
        break;
      }
    }
  }
  if(x + velocity[0] * multiplier < CP_System_GetDisplayWidth()/2.0f && x + velocity[0] * multiplier > -CP_System_GetDisplayWidth() / 2.0f)
    x += velocity[0] * multiplier;
  if (y + velocity[1] * multiplier < CP_System_GetDisplayHeight() / 2.0f && y + velocity[1] * multiplier > -CP_System_GetDisplayHeight() / 2.0f)
    y += velocity[1] * multiplier;
  return errored;
}

int checkItems(void)
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
      continue;
    float distance = sqrtf((items[i].x - x) * (items[i].x - x) + (items[i].y - y) * (items[i].y - y));
    if (distance < playerRadius + items[i].radius)
    {
      switch (items[i].type)
      {
      case 0:
        health += items[i].containes;
        break;
      case 1:
        weapon = items[i].containes;
        break;
      case 2:
        switch (items[i].containes) 
        {
        case 1:
          if (powerUpTimer <= 0)
          {
            powerUp = items[i].containes;
            powerUpTimer = 5.0f;
          }
          break;
        case 2:
          if (powerUpTimer <= 0)
          {
            powerUp = items[i].containes;
            powerUpTimer = 5.0f;
          }
          break;
        case 3:
          health = 200;
          break;
        case 4:
          for (int k = 0; k < MAX_ENEMIES; k++)
          {
            if (en[k].active == 0)
              continue;
            en[k].active = 0;
            kills++;
          }
          for (int k = 0; k < MAX_BOSSESS; k++)
          {
            if (bosses[k].active == 0)
              continue;
            bosses[k].active = 0;
            kills++;
          }
          break;
        case 5:
          if (powerUpTimer <= 0)
          {

            powerUp = items[i].containes;
            powerUpTimer = 5.0f;
          }
          break;
        }

        break;

      }
      addPickup(items[i]);
      items[i].active = 0;
    }
    items[i].life -= CP_System_GetDt();
    if (items[i].life <= 0)
      items[i].active = 0;
  }
  return 0;
}


/* Shooty Stuff */

int playerFire(void)
{
  float dir[2] = { 0,0 };
  float magnitude = 0;
  float angle = 0;
  if (weapon == 3)
  {

    for (int k = 0; k < SPREAD_COUNT; k++)
    {
      angle = CP_Random_RangeFloat(-100, 100);
      dir[0] = (CP_Input_GetMouseX() - CP_System_GetWindowWidth() / 2.0f) - x + angle;
      dir[1] = -(CP_Input_GetMouseY() - CP_System_GetWindowHeight() / 2.0f) - y + angle;
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      for (int i = 0; i < MAX_BULLETS; i++)
      {
        if (bullets[i].active == 0)
        {
          bullets[i].radius = 10;
          bullets[i].active = 1;
          bullets[i].x = x;
          bullets[i].y = y;
          bullets[i].dir[0] = dir[0];
          bullets[i].dir[1] = dir[1];
          bullets[i].life = 5;
          bullets[i].users = 1;
          if (powerUp != 1)
            bullets[i].pwr = 20;
          else if (powerUp == 1)
            bullets[i].pwr = 20000;
          bullets[i].speed = BULLET_SPEED;
          break;
        }
      }
    }
  }
  else
  {
    dir[0] = (CP_Input_GetMouseX() - CP_System_GetWindowWidth() / 2.0f) - x;
    dir[1] = -(CP_Input_GetMouseY() - CP_System_GetWindowHeight() / 2.0f) - y;
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
      if (bullets[i].active == 0)
      {
        bullets[i].radius = 10;
        bullets[i].active = 1;
        bullets[i].x = x;
        bullets[i].y = y;
        bullets[i].dir[0] = dir[0];
        bullets[i].dir[1] = dir[1];
        bullets[i].life = 5;
        bullets[i].users = 1;
        bullets[i].speed = BULLET_SPEED;
        switch (weapon)
        {
        case 0:
          if (powerUp != 1)
            bullets[i].pwr = 25;
          else if (powerUp == 1)
            bullets[i].pwr = 20000;
          break;
        case 1:
          if (powerUp != 1)
            bullets[i].pwr = 15;
          else if (powerUp == 1)
            bullets[i].pwr = 20000;
 
          break;
        case 2:
          if (powerUp != 1)
            bullets[i].pwr = 300;
          else if (powerUp == 1)
            bullets[i].pwr = 20000;
          bullets[i].speed *= 4;
          break;
        case 4:
          if (powerUp != 1)
            bullets[i].pwr = 30;
          else if (powerUp == 1)
            bullets[i].pwr = 20000;
          break;
        }
        break;
      }
    }
  }
  return 0;
}

int fireAtPlayer(enemy a)
{
  float dir[2] = { 0,0 };
  dir[0] = x - a.x;
  dir[1] = y - a.y;
  float magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
  dir[0] /= magnitude;
  dir[1] /= magnitude;
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active == 0)
    {
      bullets[i].radius = 10;
      bullets[i].active = 1;
      bullets[i].x = a.x;
      bullets[i].y = a.y;
      bullets[i].dir[0] = dir[0];
      bullets[i].dir[1] = dir[1];
      bullets[i].life = 10;
      bullets[i].pwr = CP_Random_RangeInt(5, 25);
      bullets[i].users = 0;
      bullets[i].speed = BULLET_SPEED;
      CP_Sound_PlayAdvanced(bulletSounds[0], 2.0f, CP_Random_RangeFloat(3.0f,10.0f), 0, CP_SOUND_GROUP_0);
      break;
    }
  }
  return 0;
}

int bossPatterns(enemy boss)
{
  float dir[2] = { 0 };
  float angle = 0;
  float magnitude = 0;
  switch (boss.pattern) 
  {
  case 0:
    for (int i = 0; i < BASE_PATTERN; i++) 
    {
      dir[0] = cosf(360.0f / BASE_PATTERN * i);
      dir[1] = sinf(360.0f / BASE_PATTERN * i);
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      for (int k = 0; k < MAX_BULLETS; k++)
      {
        if (bullets[k].active == 0)
        {
          bullets[i].radius = 10;
          bullets[k].active = 1;
          bullets[k].x = boss.x;
          bullets[k].y = boss.y;
          bullets[k].dir[0] = dir[0];
          bullets[k].dir[1] = dir[1];
          bullets[k].life = 10;
          bullets[k].pwr = CP_Random_RangeInt(5, 25);
          bullets[k].users = 0;
          bullets[k].speed = BULLET_SPEED;
          break;
        }
      }
    }
    break;
  case 1:

    for (int k = 0; k < SPREAD_COUNT * 3; k++)
    {
      angle = CP_Random_RangeFloat(-100, 100);
      dir[0] = x - boss.x + angle;
      dir[1] = y - boss.y + angle;
      magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
      dir[0] /= magnitude;
      dir[1] /= magnitude;
      for (int i = 0; i < MAX_BULLETS; i++)
      {
        if (bullets[i].active == 0)
        {
          bullets[i].radius = 10;
          bullets[i].active = 1;
          bullets[i].x = boss.x;
          bullets[i].y = boss.y;
          bullets[i].dir[0] = dir[0];
          bullets[i].dir[1] = dir[1];
          bullets[i].life = 5;
          bullets[i].users = 0;
          bullets[i].pwr = 20;
          bullets[i].speed = BULLET_SPEED;
          break;
        }
      }
    }

    break;
  case 2:

    dir[0] = x - boss.x ;
    dir[1] = y - boss.y ;
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    for (int i = 0; i < MAX_BULLETS; i++)
    {

      if (bullets[i].active == 0)
      {
        bullets[i].radius = 10;
        bullets[i].active = 1;
        bullets[i].x = boss.x;
        bullets[i].y = boss.y;
        bullets[i].dir[0] = dir[0];
        bullets[i].dir[1] = dir[1];
        bullets[i].life = 10;
        bullets[i].pwr = CP_Random_RangeInt(5, 25);
        bullets[i].users = 0;
        bullets[i].speed = BULLET_SPEED;
        break;
      }

    }
    break;
  case 3:

    dir[0] = (x - boss.x);
    dir[1] = (y - boss.y);
    magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    dir[0] /= magnitude;
    dir[1] /= magnitude;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
      if (bullets[i].active == 0)
      {
        bullets[i].radius = 50;
        bullets[i].active = 1;
        bullets[i].x = boss.x;
        bullets[i].y = boss.y;
        bullets[i].dir[0] = dir[0];
        bullets[i].dir[1] = dir[1];
        bullets[i].life = 10;
        bullets[i].pwr = CP_Random_RangeInt(75, 100);
        bullets[i].users = 0;
        bullets[i].speed = BULLET_SPEED/2.0f;
        break;
      }
    }

    break;
  }
  return 0;
}

int bossShoot(void)
{
  for (int i = 0; i < MAX_BOSSESS; i++)
  {
    if (bosses[i].active == 0)
      continue;
    if (bosses[i].cooldown <= 0)
    {
      bossPatterns(bosses[i]);
      switch (bosses[i].pattern)
      {
      case 0:
        bosses[i].cooldown = .75f;
        break;
      case 1:
        bosses[i].cooldown = 1.0f;
        break;
      case 2:
        bosses[i].cooldown = .1f;
        break;
      case 3:
        bosses[i].cooldown = 2.0f;
        break;
      }

    }
    float d = CP_System_GetDt();
    if (d > 1)
      bosses[i].cooldown -= .03f;
    else
      bosses[i].cooldown -= d;
  }
  return 0;
}


 /* Moving stuff */

int moveEnemies(void)
{
  for (int i = 0; i < MAX_ENEMIES; i++)
  {
    if (en[i].active == 0)
      continue;
    if (en[i].dir[0] <= 10 && en[i].dir[1] <= 10)
    {
      en[i].dir[0] = CP_Random_RangeFloat(-100, 100);
      if (en[i].x + en[i].dir[0] > CP_System_GetWindowWidth() / 2.0 || en[i].x + en[i].dir[0] < -CP_System_GetWindowWidth() / 2.0f)
        en[i].dir[0] = -en[i].dir[0];
      en[i].dir[1] = CP_Random_RangeFloat(-100, 100);
      if (en[i].y + en[i].dir[1] > CP_System_GetWindowHeight() / 2.0 || en[i].y + en[i].dir[1] < -CP_System_GetWindowHeight() / 2.0f)
        en[i].dir[1] = -en[i].dir[1];
    }
    en[i].x += en[i].dir[0] * CP_System_GetDt();
    en[i].y += en[i].dir[1] * CP_System_GetDt();
    en[i].dir[0] += -en[i].dir[0] * CP_System_GetDt();
    en[i].dir[1] += -en[i].dir[1] * CP_System_GetDt();
  }
  return 0;
}

int moveBosses(void)
{
  for (int i = 0; i < MAX_BOSSESS; i++)
  {
    if (bosses[i].active == 0)
      continue;
    if (bosses[i].dir[0] <= 10 && bosses[i].dir[1] <= 10)
    {
      bosses[i].dir[0] = CP_Random_RangeFloat(-100, 100);
      if (bosses[i].x + bosses[i].dir[0] > CP_System_GetWindowWidth() / 2.0 || bosses[i].x + bosses[i].dir[0] < -CP_System_GetWindowWidth() / 2.0f)
        bosses[i].dir[0] = -bosses[i].dir[0];
      bosses[i].dir[1] = CP_Random_RangeFloat(-100, 100);
      if (bosses[i].y + bosses[i].dir[1] > CP_System_GetWindowHeight() / 2.0 || bosses[i].y + bosses[i].dir[1] < -CP_System_GetWindowHeight() / 2.0f)
        bosses[i].dir[1] = -bosses[i].dir[1];
    }
    bosses[i].x += bosses[i].dir[0] * CP_System_GetDt();
    bosses[i].y += bosses[i].dir[1] * CP_System_GetDt();
    bosses[i].dir[0] += -bosses[i].dir[0] * CP_System_GetDt();
    bosses[i].dir[1] += -bosses[i].dir[1] * CP_System_GetDt();
  }
  return 0;
}

int moveBullets(void)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active)
    {
      bullets[i].x += (bullets[i].dir[0] * bullets[i].speed);
      bullets[i].y += (bullets[i].dir[1] * bullets[i].speed);
      bullets[i].life -= CP_System_GetDt();
      if (bullets[i].life <= 0)
      {
        bullets[i].active = 0;
        bullets[i].users = 0;
        bullets[i].speed = BULLET_SPEED;
      }
      float distance = sqrtf((bullets[i].x - x) * (bullets[i].x - x) + (bullets[i].y - y) * (bullets[i].y - y));
      if (distance < playerRadius && bullets[i].users == 0)
      {
        if (powerUp == 5)
          continue;
        health -= bullets[i].pwr;
        bullets[i].active = 0;
        bullets[i].speed = BULLET_SPEED;
      }
      else if (bullets[i].users == 1)
      {
        for (int k = 0; k < MAX_ENEMIES; k++)
        {
          if (en[k].active == 0)
            continue;
          float distance2 = sqrtf((bullets[i].x - en[k].x) * (bullets[i].x - en[k].x) + (bullets[i].y - en[k].y) * (bullets[i].y - en[k].y));
          if (distance2 < en[k].radius/2.0f)
          {
            en[k].health -= bullets[i].pwr;
            bullets[i].active = 0;
            bullets[i].speed = BULLET_SPEED;
            if (en[k].health <= 0)
            {
              kills++;
              if (CP_Random_RangeInt(0, 100) < 75)
              {
                float temploc[2] = { en[k].x, en[k].y };
                dropItem(temploc);
              }
              en[k].active = 0;

            }
          }
        }
        for (int k = 0; k < MAX_BOSSESS; k++) 
        {
          if (bosses[k].active == 0)
            continue;
          float distance2 = sqrtf((bullets[i].x - bosses[k].x) * (bullets[i].x - bosses[k].x) + (bullets[i].y - bosses[k].y) * (bullets[i].y - bosses[k].y));
          if (distance2 < bosses[k].radius / 2.0f)
          {
            bosses[k].health -= bullets[i].pwr;
            bullets[i].active = 0;
            bullets[i].speed = BULLET_SPEED;
            if (bosses[k].health <= 0)
            {
              kills++;
              if (CP_Random_RangeInt(0, 100) < 75)
              {
                float temploc[2] = { bosses[k].x, bosses[k].y };
                dropItem(temploc);
              }
              bosses[k].active = 0;

            }
          }
        }
      }
    }
  }
  return 0;
}

int enemyShoot(void)
{
  for (int i = 0; i < MAX_ENEMIES; i++)
  {
    if (en[i].active == 0)
      continue;
    if (en[i].cooldown <= 0)
    {
      fireAtPlayer(en[i]);
      switch (en[i].type)
      {
      case 0:
        en[i].cooldown = 5.0f;
        break;
      case 1:
        en[i].cooldown = .5f;
        break;
      }
    }
    float d = CP_System_GetDt();
    if (d > 1)
      en[i].cooldown -= .03f;
    else
      en[i].cooldown -= d;
  }
  return 0;
}


// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gameLoopUpdate(void)
{
  if (health <= 0)
  {
    CP_Engine_SetNextGameState(endScreenInit, endScreenUpdate, endScreenExit);
    return;
  }

  CP_Graphics_ClearBackground(CP_Color_Create(117, 117, 117, 255));
  addTime(CP_System_GetDt());
  char buffer[15];
  snprintf(buffer, sizeof buffer, "%.3f", getTime());
  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 25));
  CP_Settings_TextSize(300 - (getTime() - floorf(getTime())) * 125.0f);
  CP_Font_DrawText(buffer, CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f);
  CP_Settings_Fill(CP_Color_Create(50, 50, 50, 50));
  CP_Settings_TextSize(200);
  CP_Font_DrawText(buffer, CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f);

  direction[0] = CP_Input_GetMouseX() - ((CP_System_GetWindowWidth() / 2.0f) + x);
  direction[1] = CP_Input_GetMouseY() - ((CP_System_GetWindowHeight() / 2.0f) - y);
  rot = (float)(atan2(direction[1], direction[0]) * (180.0f / 3.14159265f) + 90.0f);


  /* Draw all on screen items */
  drawBullets();
  drawPlayer();
  drawEnemies();
  drawItems();
  drawBosses();
  drawPickupText();

  /* Check  enemy shooting */
  enemyShoot();
  bossShoot();

  /* Move on screen items    */
  moveEnemies();
  moveBullets();
  moveBosses();

  /* Check user input and collisions */
  checkKeys();
  checkItems();

  CP_Settings_Fill(CP_Color_Create(139, 50, 77, (200 - health)));
  CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f, CP_System_GetWindowWidth() + 100.0f, CP_System_GetWindowHeight() + 100.0f);
  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
  addTimer -= CP_System_GetDt();
  if (cooldown > 0)
    cooldown -= CP_System_GetDt();
  drawWeapon();
  if (addTimer <= 0)
  {
    addEnemy();
    addTimer = 5.0f / cbrtf((float)kills + 1.0f);
  }
  if (getTime() >= 60.0f)
    bossesEnabled = 1;
  multiplier -= CP_System_GetDt() * 5;
  if (multiplier < 1)
  {
    multiplier = 1;
  }
  if (powerUpTimer > 0)
    powerUpTimer -= CP_System_GetDt();
  if (powerUpTimer <= 0)
    powerUp = 0;
  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
  CP_Settings_TextSize(100);
  if (getTime() < 5.0f)
    CP_Font_DrawText("Your Goal: Live long", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f);
  else if (getTime() < 10.0f)
  {
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255- (int)(50.0f * (5 - (10-getTime())))));
    CP_Font_DrawText("Your Goal: Live long", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f);
  }

}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gameLoopExit(void)
{
  for (int i = 0; i < AUDIOS; i++) 
  {
    CP_Sound_Free(&bulletSounds[i]);
  }
}
