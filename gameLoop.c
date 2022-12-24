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
#include "worldInit.h"
#include "drawStuff.h"
#include "addStuff.h"
#include "moveStuff.h"
#include "shootStuff.h"
#include "checkStuff.h"
#include "debugInfo.h"
#include "playerInv.h"
#include "globalImages.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* |---------------------- Game Stuff --------------------| */

/* Input Keys */
int playerKeys[KEY_COUNT] = { KEY_W, KEY_A, KEY_S, KEY_D, KEY_LEFT_SHIFT, KEY_SPACE, KEY_E, KEY_I, KEY_Q, KEY_R };

/* Bools */
bool bossesEnabled = false;
bool isPaused = false;
bool invOpen = false;
bool wheelOpen = false;
bool invHovered = false;

/* standard Data*/
int screen = 0;
float addTimer = 5.0f;
float multiplier = 1;

/* Arrays, Change to Linked lists when optimizing */
static enemy* enHead = NULL; 
static bullet* head = NULL;
item items[MAX_DROPS] = { 0 };
notiString pickupText[MAX_TEXT] = { 0 };
CP_Sound bulletSounds[AUDIOS] = { 0 };
player pl = { 0 };
camera c = { 0 };
building buildings[NUMBER_OF_BUILDINGS] = { 0 };

camera *retCam(void) 
{
  return &c;
}

player* returnPlayer(void) 
{
  return &pl;
}

bool returnInvSel(void) 
{
  return invHovered;
}

void disableWheel(void) 
{
  wheelOpen = false;
}
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gameLoopInit(void)
{

  CP_Settings_RectMode(CP_POSITION_CENTER);
  CP_Settings_EllipseMode(CP_POSITION_CENTER);
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_REPEAT);
  bossesEnabled = 0;
  initScrollable();
  //initBuildings(buildings);
  initAudio(bulletSounds);
  initPlayer(&pl, &multiplier, &addTimer);
  initDrops(items);
  initImages();

  for(int i = 0; i < WHEEL_SIZE; i++)
  {
    removeFromWheel(i);
  }  
  while (returnHead())
  {
    freeItem(0);
  }
  addItem(0, 1);
  addToWheel(returnItemAtPos(0), 0, 0);
  swatchActive(0, &pl);
  initializeAmmo(&pl);
  addItem(2, retAmmo()->lightStorage);

  /*while (checkInsideBuilding(buildings, 0, pl) != 0)
  {
    pl.x = CP_Random_RangeFloat(-1000, 1000);
    pl.y = CP_Random_RangeFloat(-1000, 1000);
  }*/
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gameLoopUpdate(void)
{

  CP_Settings_Stroke(CP_Color_CreateHex(0x000000ff));
  drawBackGroundLayer();
  if (pl.health <= 0)
  {
    CP_Engine_SetNextGameState(endScreenInit, endScreenUpdate, endScreenExit);
    return;
  }

  CP_Graphics_ClearBackground(CP_Color_Create(117, 117, 117, 255));
  addTime(CP_System_GetDt());

  pl.direction[0] = CP_Input_GetMouseX()- ((SCREEN_WIDTH / 2.0f) + (pl.x - c.x));
  pl.direction[1] = CP_Input_GetMouseY() - ((SCREEN_HEIGHT / 2.0f) - (pl.y - c.y));
  pl.rot = (float)(atan2(pl.direction[1], pl.direction[0]) * (180.0f / 3.14159265f) + 90.0f);
  c.x = pl.x;
  c.y = pl.y;

  /* Draw all on screen items */
  drawBullets(head,c);
  drawPlayer(pl,c);
  drawEnemies(&enHead, c);
  drawItems(items,c);;
  drawPickupText(pickupText,c);
  //drawBuildings(buildings,c);

  if (invOpen)
    invHovered = drawInventory(returnHead());
  else
    invHovered = false;
  if (wheelOpen)
    drawWheel(&pl);
  drawAmmo(&pl, invOpen, wheelOpen );

  CP_Settings_StrokeWeight(0);
  /* Check  enemy shooting */
  enemyShoot(enHead, bulletSounds, &head, pl);

  /* Move on screen items    */
  moveEnemies(enHead, buildings);
  moveBullets(&head, &enHead,  &pl, items, buildings);

  int check = /*checkAgainstBuilding(buildings, 0, &pl)*/0;

  /* Check user input and collisions */
  checkKeys(&pl, &multiplier, &head, playerKeys, &invOpen, &wheelOpen, isPaused, check);
  checkItems(items, &pl, enHead, pickupText);

  CP_Settings_Fill(CP_Color_Create(139, 50, 77, (200 - pl.health)));
  CP_Graphics_DrawRect(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT + 100.0f);
  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
  addTimer -= CP_System_GetDt();
  if (pl.cooldown > 0)
    pl.cooldown -= CP_System_GetDt();
  //drawWeapon(pl.weapon, pl.powerUpTimer, pl.powerUp);

  if (addTimer <= 0)
  {
    int type;
    if (bossesEnabled)
    {
      type = CP_Random_RangeInt(0, ENEMY_TYPE);
      enHead = setEnemyStats(addEnemy(&enHead), c, type);
    }
    else
    {
      while (type = CP_Random_RangeInt(0, ENEMY_TYPE), type == 2);
      enHead = setEnemyStats(addEnemy(&enHead), c, type);
    }
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
  if (pl.weapon->reloadClock > 0)
    pl.weapon->reloadClock -= CP_System_GetDt();
  else if (pl.weapon->reloadClock < 0)
    pl.weapon->reloadClock = 0;
  drawDebugInfo(&pl, enHead);

}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gameLoopExit(void)
{
  for (int i = 0; i < AUDIOS; i++) 
  {
    CP_Sound_Free(&bulletSounds[i]);
  }
  freeImages();
  releaseScrollable();
}
