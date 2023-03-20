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
// Copyright � 2020 DigiPen, All rights reserved.
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
#include "worldSystems.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* |---------------------- Game Stuff --------------------| */

/* Input Keys */

/* Bools */
bool bossesEnabled = false;
bool invOpen = false;
bool wheelOpen = false;
bool invHovered = false;

/* standard Data*/
int screen = 0;
int wave = 0;
int enemiesPerWave = 5;
int enemiesAlive = 5;
float addTimer = 5.0f;
float multiplier = 1;

/* rapidly modifying Arrays, Change to Linked lists when optimizing */
static enemy* enHead = NULL; 
static bullet* head = NULL;
item items[MAX_DROPS] = { 0 };
notiString pickupText[MAX_TEXT] = { 0 };

/* Static arrays */
CP_Sound bulletSounds[AUDIOS] = { 0 };
objective obis[MAX_OBJECTIVES] = { 0 };


player pl = { 0 };
camera c = { 0 };
building* buildings = NULL;

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

void increaseAlive(void)
{
  enemiesAlive++;
}

int getAlive(void) 
{
  return enemiesAlive;
}

void decreaseAlive(void) 
{
  --enemiesAlive;
}

objective* returnObis(void) 
{
  return obis;
}

void disableWheel(void) 
{
  wheelOpen = false;
}
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gameLoopInit(void)
{
  initImages();
  initScrollable();
  if(getGame())
    return;
  obis[0].title = "Survive";
  setGame(true);
  bossesEnabled = 0;

  generateWorld();


  buildings = returnBuildings();
  initAudio(bulletSounds);
  initPlayer(&pl, &multiplier, &addTimer);
  initDrops(items);
  wave = 1;
  enemiesPerWave = 5;
  addTimer = 0;
  for(int i = 0; i < WHEEL_SIZE; i++)
  {
    removeFromWheel(i);
  }  
  while (returnHead())
  {
    InvItem* oldHead = returnHead();
    
    removeItem(0);
    freeItem(oldHead);
  }
  while (enHead) 
  {
    enemy* e = enHead;
    enHead = e->next;
    removeEnemy(e);
  }
  while (head) 
  {
    bullet* b = head;
    head = b->next;
    removeBullet(b);
  }
  addItem(0, 1);
  addToWheel(returnItemAtPos(0), 0, 0);
  swatchActive(0, &pl);
  initializeAmmo(&pl);
  addItem(2, retAmmo()->lightStorage);

  while (checkInsideBuilding(buildings, 0, pl) != 0)
  {
    pl.x = CP_Random_RangeFloat(-1000, 1000);
    pl.y = CP_Random_RangeFloat(-1000, 1000);
  }
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gameLoopUpdate(void)
{

  CP_Settings_Stroke(BLACK);
  drawBackGroundLayer(&pl);
  if (pl.health <= 0)
  {
    CP_Engine_SetNextGameState(endScreenInit, endScreenUpdate, endScreenExit);
    return;
  }

  CP_Graphics_ClearBackground(CP_Color_Create(117, 117, 117, 255));


  pl.direction.x = CP_Input_GetMouseX() - ((SCREEN_WIDTH / 2.0f) + (pl.x - c.x));
  pl.direction.y = CP_Input_GetMouseY() - ((SCREEN_HEIGHT / 2.0f) - (pl.y - c.y));
  pl.rot = (float)(atan2(pl.direction.y, pl.direction.x) * (180.0f / 3.14159265f) + 90.0f);
  c.x = pl.x;
  c.y = pl.y;

  /* Draw all on screen items */
  drawBuildings(buildings, c);
  drawBullets(head,c);
  drawPlayer(pl,c);
  drawEnemies(&enHead, c);
  drawItems(items,c);
  drawPickupText(pickupText,c);
  drawObjectiveBoard();


  if (invOpen)
    invHovered = drawInventory(returnHead());
  else
    invHovered = false;

  if (wheelOpen)
    drawWheel(&pl);
  
  drawAmmo(&pl, invOpen, wheelOpen );
  if (getPause())
    return;
  addTime(CP_System_GetDt());

  CP_Settings_StrokeWeight(0);
  /* Check  enemy shooting */
  enemyShoot(enHead, bulletSounds, &head, pl);

  /* Move on screen items    */
  moveEnemies(&enHead, buildings);
  moveBullets(&head, &enHead,  &pl, items, buildings);

  int check = checkAgainstBuilding(buildings, 0, &pl);

  /* Check user input and collisions */
  checkKeys(&pl, &multiplier, &head, &invOpen, &wheelOpen, getPause(), check);
  checkItems(items, &pl, enHead, pickupText);

  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
  if(enemiesAlive <= 0)
    addTimer -= CP_System_GetDt();
  if (pl.cooldown > 0)
    pl.cooldown -= CP_System_GetDt();
  //drawWeapon(pl.weapon, pl.powerUpTimer, pl.powerUp);

  if (addTimer <= 0)
  {
    for (int i = 0; i < enemiesPerWave; i++) 
    {
      int type;
      if (bossesEnabled)
      {
        type = CP_Random_RangeInt(0, ENEMY_TYPE);

        enHead = setEnemyStats(addEnemy(&enHead), c, type);
        if (checkInsideBuilding(buildings, 1, enHead))
        {
          enemy* old = enHead;
          enHead = enHead->next;
          removeEnemy(old);
        }
      }
      else
      {
        while (type = CP_Random_RangeInt(0, ENEMY_TYPE), type == 2);
        enHead = setEnemyStats(addEnemy(&enHead), c, type);
        if (checkInsideBuilding(buildings, 1, enHead))
        {
          enemy* old = enHead;
          enHead = enHead->next;
          removeEnemy(old);
        }
      }
    }
    enemiesPerWave += (int)(5.0f * (1.0f + cbrtf(wave - 2.0f)));
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
  char buffer[30];
  snprintf(buffer, _countof(buffer), "%i", enemiesAlive);
  CP_Font_DrawText(buffer, SCREEN_WIDTH/2.0f, 100);
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gameLoopExit(void)
{
  if (getGame())
    return;

  for (int i = 0; i < AUDIOS; i++) 
  {
    CP_Sound_Free(&bulletSounds[i]);
  }
  freeImages();
  releaseScrollable();
  
  InvItem* i = returnHead();
  if (i != NULL)
  {
    InvItem* next = i->next;
    while (i)
    {
      next = i->next;
      freeItem(i);
      i = next;
    }

  }
  resetHead();
  if(getGenerated())
    free(getWorldName());
}
