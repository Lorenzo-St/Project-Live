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
#include "worldInit.h"
#include "drawStuff.h"
#include "addStuff.h"
#include "moveStuff.h"
#include "shootStuff.h"
#include "checkStuff.h"
/* |---------------------- Game Stuff --------------------| */

/* Gameworld and enemy stats and storage */
int bossesEnabled = 0;
int screen = 0;
int isPaused = 0;
float addTimer = 5.0f;
float multiplier = 1;
enemy en[MAX_ENEMIES] = { 0 };
enemy bosses[MAX_BOSSESS] = { 0 };
bullet bullets[MAX_BULLETS] = { 0 };
item items[MAX_DROPS] = { 0 };
string pickupText[MAX_TEXT] = { 0 };
CP_Sound bulletSounds[AUDIOS] = { 0 };
player pl = { 0 };

/*Player statsand data storage items */

int playerKeys[KEY_COUNT] = { 87, 65, 83, 68, 340, 32 };

int initPlayer(void)
{
  pl.powerUp = 0;
  pl.powerUpTimer = 0.0f;
  pl.x, pl.y = 0;
  pl.move_speed = 100;
  pl.cooldown = 1.0f;
  addTimer = 5.0f;
  pl.kills = 0;
  pl.weapon = 0;
  pl.health = 200;
  pl.playerRadius = 20;
  multiplier = 1;
  setTime(0.0f);
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
  initAudio(bulletSounds);
  initPlayer();
  initEnemies(en);
  initBullets(bullets);
  initDrops(items);
  initBosses(bosses);
}

void drawPlayer(void)
{
  CP_Settings_StrokeWeight(1.5f);
  CP_Settings_Fill(CP_Color_CreateHex(0x99A3A4));
  CP_Graphics_DrawCircle(pl.x + (CP_System_GetWindowWidth() / 2.0f), -pl.y + (CP_System_GetWindowHeight() / 2.0f), pl.playerRadius * 2);
  CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
  CP_Graphics_DrawRectAdvanced((pl.x + ((pl.direction[0] * pl.playerRadius) / sqrtf(pl.direction[0] * pl.direction[0] + pl.direction[1] * pl.direction[1]))) + (CP_System_GetWindowWidth() / 2.0f), (-pl.y + ((pl.direction[1] * pl.playerRadius) / sqrtf(pl.direction[0] * pl.direction[0] + pl.direction[1] * pl.direction[1]))) + (CP_System_GetWindowHeight() / 2.0f), pl.playerRadius / 1.25f, 10, pl.rot, 0.0f);
}



// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gameLoopUpdate(void)
{
  if (pl.health <= 0)
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

  pl.direction[0] = CP_Input_GetMouseX() - ((CP_System_GetWindowWidth() / 2.0f) + pl.x);
  pl.direction[1] = CP_Input_GetMouseY() - ((CP_System_GetWindowHeight() / 2.0f) - pl.y);
  pl.rot = (float)(atan2(pl.direction[1], pl.direction[0]) * (180.0f / 3.14159265f) + 90.0f);


  /* Draw all on screen items */
  drawBullets(bullets);
  drawPlayer();
  drawEnemies(en);
  drawItems(items);
  drawBosses(bosses);
  drawPickupText(pickupText);

  /* Check  enemy shooting */
  enemyShoot(en, bulletSounds, bullets, pl);
  bossShoot(bosses, pl, bullets);

  /* Move on screen items    */
  moveEnemies(en);
  moveBullets(bullets, en, bosses, &pl, items);
  moveBosses(bosses);

  /* Check user input and collisions */
  checkKeys(pl, multiplier, bullets, playerKeys, isPaused);
  checkItems(items, pl, en, bosses, pickupText);

  CP_Settings_Fill(CP_Color_Create(139, 50, 77, (200 - pl.health)));
  CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f, CP_System_GetWindowWidth() + 100.0f, CP_System_GetWindowHeight() + 100.0f);
  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
  addTimer -= CP_System_GetDt();
  if (pl.cooldown > 0)
    pl.cooldown -= CP_System_GetDt();
  drawWeapon(pl.weapon, pl.powerUpTimer, pl.powerUp);
  if (addTimer <= 0)
  {
    addEnemy(bossesEnabled,en, bosses);
    addTimer = 5.0f / cbrtf((float)pl.kills + 1.0f);
  }
  if (getTime() >= 60.0f)
    bossesEnabled = 1;
  multiplier -= CP_System_GetDt() * 5;
  if (multiplier < 1)
  {
    multiplier = 1;
  }
  if (pl.powerUpTimer > 0)
    pl.powerUpTimer -= CP_System_GetDt();
  if (pl.powerUpTimer <= 0)
    pl.powerUp = 0;
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
