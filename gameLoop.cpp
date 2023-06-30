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
#include <vector>
#include "playerInfo.h"
#include "playerInput.h"
extern "C"
{
#define CPP
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
#include "EnemyInfo.h"

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
  bool boxClosed = false;

#ifdef _DEBUG
  bool invicible = false;


#endif // DEBUG


  /* standard Data*/
  int screen = 0;
  int wave = 0;
  int enemiesPerWave = 5;
  int enemiesAlive = 5;
  float addTimer = 5.0f;
  float multiplier = 1;
  int maximumEnemies = 10;

  /* rapidly modifying Arrays, Change to Linked lists when optimizing */
  static std::vector<enemy> enemies;
  static bullet* head = NULL;
  static std::vector<item> items;
  notiString pickupText[MAX_TEXT] = { 0 };

  /* Static arrays */
  CP_Sound bulletSounds[AUDIOS] = { 0 };
  objective obis[MAX_OBJECTIVES] = { 0 };

  int getMaxEnemies(void)
  {
    return maximumEnemies;
  }

  player* pl = nullptr;
  building* buildings = NULL;
  camera c;
  player* returnPlayer(void) 
  {
    return pl;
  }

  camera* retCam() 
  {
    return &c;
  }

  bool* retWheel(void)
  {
    return &wheelOpen;
  }

  bool* retInventory(void)
  {
    return &invOpen;
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
    if (getGame())
      return;
    obis[0].title = "Survive";
    setGame(true);
    bossesEnabled = 0;

    generateWorld();

    boxClosed = false;

    buildings = returnBuildings();
    initAudio(bulletSounds);
    pl = createPlayer();
    items.clear();
    wave = 1;
    enemiesPerWave = 5;
    addTimer = 0;
    resetWheel();
    resetHead();
    enemies.clear();
    while (head)
    {
      bullet* b = head;
      head = b->next;
      removeBullet(b);
    }
    addItem(0, 1);

    addToWheel(returnItemAtPos(0), 0, 0);
    swatchActive(0, pl);
    initializeAmmo(pl);
    addItem(2, retAmmo()->lightStorage);
    addItem(1, 1);
    addItem(2, 1);
    addItem(3, 1);
    addItem(4, 1);
    addItem(5, 1);
    addItem(7, 1);
    addItem(8, 1);
    addItem(9, 1);
    addItem(10, 1);
    addItem(11, 1);
    enemiesAlive = 0;
    while (checkInsideBuilding(buildings, 0, pl) != 0)
    {
      CP_Vector pos = { CP_Random_RangeFloat(-1000, 1000), CP_Random_RangeFloat(-1000, 1000) };
      PlayerSetPosition(pl, &pos);
    }
  }

  // use CP_Engine_SetNextGameState to specify this function as the update function
  // this function will be called repeatedly every frame
  void gameLoopUpdate(void)
  {

    CP_Settings_Stroke(BLACK);
    drawBackGroundLayer(pl);
    if (PlayerGetHealth(pl) <= 0)
    {
      CP_Engine_SetNextGameState(endScreenInit, endScreenUpdate, endScreenExit);
      return;
    }

    CP_Graphics_ClearBackground(CP_Color_Create(117, 117, 117, 255));
    c.x = PlayerGetPos(pl)->x;
    c.y = PlayerGetPos(pl)->y;

    PlayerUpdate(pl);
    /* Draw all on screen items */
    drawBullets(head, &c);
    drawPlayer(pl, &c);
    drawEnemies(enemies, &c);
    drawBuildings(buildings, &c);
    drawDirector(enemies);
    drawItems(items, &c);
    drawPickupText(pickupText, &c);
    drawObjectiveBoard();
    if (boxClosed == false)
    {
      CP_Settings_Fill(WHITE);
      CP_Settings_Stroke(BLACK);
      CP_Settings_StrokeWeight(5);

      CP_Graphics_DrawRectAdvanced(SCREEN_WIDTH / 2.0f + 100 * (SCREEN_WIDTH / 1920.0f), SCREEN_HEIGHT / 2.0f - 100.0f * (SCREEN_WIDTH / 1920.0f), 700 * (SCREEN_WIDTH / 1920.0f), 200 * (SCREEN_WIDTH / 1920.0f), 0, 10);
      CP_Settings_TextSize(25 * (SCREEN_WIDTH / 1920.0f));
      CP_Settings_Fill(BLACK);
      CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
      CP_Font_DrawText("Hey there!", SCREEN_WIDTH / 2.0f - 230 * (SCREEN_WIDTH / 1920.0f), SCREEN_HEIGHT / 2.0f - 170.0f * (SCREEN_WIDTH / 1920.0f));
      CP_Font_DrawText("A lot of enemies are on the way!", SCREEN_WIDTH / 2.0f - 230 * (SCREEN_WIDTH / 1920.0f), SCREEN_HEIGHT / 2.0f - 150.0f * (SCREEN_WIDTH / 1920.0f));
      CP_Font_DrawText("You'll need to collect what they drop to improve your stuff", SCREEN_WIDTH / 2.0f - 230 * (SCREEN_WIDTH / 1920.0f), SCREEN_HEIGHT / 2.0f - 130.0f * (SCREEN_WIDTH / 1920.0f));
      CP_Font_DrawText("Use \"E\" to access your inventory", SCREEN_WIDTH / 2.0f - 230 * (SCREEN_WIDTH / 1920.0f), SCREEN_HEIGHT / 2.0f - 110.0f * (SCREEN_WIDTH / 1920.0f));
      CP_Font_DrawText("Use \"Q\" to access your weapon wheel", SCREEN_WIDTH / 2.0f - 230 * (SCREEN_WIDTH / 1920.0f), SCREEN_HEIGHT / 2.0f - 90.0f * (SCREEN_WIDTH / 1920.0f));
      CP_Font_DrawText("You can upgrade your equiped weapons with any parts you find", SCREEN_WIDTH / 2.0f - 230 * (SCREEN_WIDTH / 1920.0f), SCREEN_HEIGHT / 2.0f - 70.0f * (SCREEN_WIDTH / 1920.0f));
      CP_Settings_TextSize(20 * (SCREEN_WIDTH / 1920.0f));
      CP_Font_DrawText("press anything to continue...", SCREEN_WIDTH / 2.0f - 230 * (SCREEN_WIDTH / 1920.0f), SCREEN_HEIGHT / 2.0f - 40.0f * (SCREEN_WIDTH / 1920.0f));

      CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

      if (isPressed(Any))
        boxClosed = true;
      return;
    }
    if (invOpen)
      invHovered = drawInventory(returnHead());
    else
      invHovered = false;

    if (getTime() < 3.0f) 
    {
      PlayerSetInvincible(pl, true);
    }
    else if(PlayerIsInvincible(pl))
    {
      PlayerSetInvincible(pl, false);
    }

    if (wheelOpen)
      drawWheel(pl);

    drawAmmo(pl, invOpen, wheelOpen);
    if (getPause() && boxClosed)
      return;
    addTime(CP_System_GetDt());

    CP_Settings_StrokeWeight(0);
    /* Check  enemy shooting */
    enemyShoot(enemies, bulletSounds, &head, pl);

    /* Move on screen items    */
    moveEnemies(enemies, buildings);
    moveBullets(&head, enemies, pl, items, buildings);

    int check = checkAgainstBuilding(buildings, 0, pl);

    /* Check user input and collisions */
    checkKeys(pl,&multiplier, &head, &invOpen, &wheelOpen, getPoPause(), check);
    checkItems(items, pl, enemies.data(), pickupText);

    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    if (enemiesAlive <= 0)
    {
      addTimer -= CP_System_GetDt();
      ColorMode cm = getColorMode();
      if (cm == DarkMode)
        CP_Settings_Fill(WHITE);
      else if (cm == LightMode)
        CP_Settings_Fill(BLACK);

      char buff[15];
      CP_Settings_TextSize(60);
      snprintf(buff, _countof(buff), "New Wave in: %2.3f", addTimer);
      CP_Font_DrawText(buff, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 5.0f);
    }
    //drawWeapon(pl.weapon, pl.powerUpTimer, pl.powerUp);
#ifdef _DEBUG
    if (CP_Input_KeyTriggered(KEY_K)) 
    {
      enemiesAlive = 0;
      enemies.clear();
    }
    if (CP_Input_KeyTriggered(KEY_I))
      invicible = true;
    if (invicible)
      PlayerSetInvincible(pl, true);
#endif
    if (addTimer <= 0)
    {
      static int totalEnemyCount = static_cast<int>(getEnemyCount());
      while (enemiesAlive < enemiesPerWave)
      {
        enemy newest = {};
        int type;
        
        if (wave < 4)
          type = 0;
        else
          type = CP_Random_RangeInt(0, (totalEnemyCount > wave/4) ? wave / 4 : totalEnemyCount);

        const EnemyInfo* eI = getEnemy(type);
        while (eI == nullptr) eI = getEnemy(0);
        newest.MaxHealth = eI->MaxHealth;
        newest.radius = eI->radius;
        newest.speed = eI->speed;
        newest.type = type;
        newest.weapon = new weaponData(*eI->weapon);
        newest.alive = true;
        for (auto const& spec : eI->stats) 
        {
          switch (spec) 
          {
          case Rapid:
            newest.weapon->attackSpeed /= 10;
            break;
          }
        }
        setEnemyStats(&newest, eI,  c, type, wave);
        if (!checkInsideBuilding(buildings, 1, &newest))
        {
          bool set = false;
          for (auto& e : enemies) 
          {
            if (e.alive == false)
            {
              e = newest;
              set = true;
              break; 
            }
          }
          if (set == false) 
            enemies.push_back(newest);
          ++enemiesAlive;
        }
      }
      if (enemiesAlive == enemiesPerWave)
      {
        float wCheck = ((wave - 2.0f) > 0) ? wave - 2.0f : 0;
        enemiesPerWave += (int)(3.0f * (1.0f + (cbrtf(wCheck)) / (wCheck != 0 ? wCheck : 1)));

        addTimer = 5.0f;
        PlayerUpdateMaxHealth(pl, (10 * (1 + wave / 100.0f)));
        wave++;
      }
      for (auto& e : enemies) 
      {
        e.health = e.MaxHealth;
      }
    }

    if (wave > 10 && !bossesEnabled)
      bossesEnabled = true;
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_TextSize(100);

#if _DEBUG && 1
    if (CP_Input_KeyTriggered(CP_KEY('K')))
      addItem(6, 1);
    drawDebugInfo(pl, enemies);
    CP_Settings_TextSize(80);
    CP_Settings_Fill(BLACK);
    char buffer[30];
    snprintf(buffer, _countof(buffer), "%i", enemiesAlive);
    CP_Font_DrawText(buffer, SCREEN_WIDTH / 2.0f, 100);
#endif
    //verifyEnemyCount(enemies.data());
    cleanInventory();
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
    resetHead();
    enemies.clear();
  }
}
