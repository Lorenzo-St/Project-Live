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

#include "cprocessing.h"
#include "mainMenu.h"
#include "options.h"
#include "gameLoop.h"
#include "credits.h"
#include "structs.h"
#include "drawStuff.h"
#include "globalImages.h"
#include "globalData.h"
#include "checkStuff.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUTTONS 4

button buttons[BUTTONS] = { 0 };

static player p = { 0 };
static int screen = 0;
static bool inWorlds = false;



struct worldButton
{
  char name[30];
};

int worldsCount = 0;
int worldsScreen = 0;
static struct worldButton* first;

void loadWorldNames(void) 
{
  if (first != NULL)
    return;
  FILE* fp;
  errno_t a = fopen_s(&fp, "./Managed/worlds.dat", "r");
  if (a != 0)
    return;
  a = fscanf_s(fp, "%i", &worldsCount);
  int places = 0;
  int base = 1;
  first = malloc(sizeof(struct worldButton) * worldsCount);
  if (first == NULL)
    return;

  while (worldsCount % base != worldsCount)
  {
    base *= 10;
    places++;
  }
  fseek(fp, places + 1 , SEEK_SET);
  for (int i = 0; i < worldsCount; i++) 
  {
    *(first + i) = (struct worldButton){ 0 };
    struct worldButton* o = first + i;

    fgets((o->name), sizeof((first + i)->name), fp);
  }
  fclose(fp);
}


void drawWorlds(void) 
{
  if (CP_Input_KeyTriggered(KEY_ESCAPE)) 
  {
    inWorlds = false;
    return;
  }

  if (checkMouseBoxCollide(SCREEN_WIDTH - 60 - SCREEN_WIDTH / 2.0f, -(60.0f - SCREEN_HEIGHT / 2.0f), 100, 100))
  {
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
      inWorlds = false;
      return;
    }
    CP_Settings_Fill(RED);
  }
  else
  {
    CP_Settings_Fill(WHITE);
  }
  CP_Graphics_DrawRect(SCREEN_WIDTH - 60.0f, 60.0f, 100, 100);
  CP_Settings_Fill(BLACK);
  CP_Settings_TextSize(25);
  CP_Font_DrawText("Return", SCREEN_WIDTH - 60.0f, 60.0f);

  float buttonWidth = SCREEN_WIDTH / 4.0f;
  float buttonHeight = SCREEN_HEIGHT / 6.0f;
  float baseX = SCREEN_WIDTH / 3.0f;
  float baseY = SCREEN_HEIGHT / 4.0f;

  int currentButton = worldsScreen * 6;
  CP_Settings_TextSize(50);
  for (int i = 0; i < 3; i++) 
  {
    if (currentButton >= worldsCount )
      break;
    if (checkMouseBoxCollide(baseX - SCREEN_WIDTH / 2.0f, -(baseY + (baseY * i) - SCREEN_HEIGHT/2.0f), buttonWidth, buttonHeight)) 
    {
      if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) 
      {
        inWorlds = false;
        setWorldName((first + currentButton)->name);
        CP_Engine_SetNextGameState(gameLoopInit, gameLoopUpdate, gameLoopExit);

      }
    
      CP_Settings_Fill(GRAY_BUT);
    }else
      CP_Settings_Fill(GRAY);

    CP_Graphics_DrawRect(baseX, baseY + (baseY * i), buttonWidth, buttonHeight);
    CP_Settings_Fill(WHITE);
    CP_Font_DrawText((first + currentButton)->name, baseX, baseY + (baseY * i));
    currentButton++;
  }

  for (int i = 0; i < 3; i++)
  {
    if (currentButton >= worldsCount)
      break;
    if (checkMouseBoxCollide(2 * baseX - SCREEN_WIDTH / 2.0f, -(baseY + (baseY * i) - SCREEN_HEIGHT / 2.0f), buttonWidth, buttonHeight))
    {
      if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      {
        inWorlds = false;
        setWorldName((first + currentButton)->name);
        CP_Engine_SetNextGameState(gameLoopInit, gameLoopUpdate, gameLoopExit);
      }

      CP_Settings_Fill(GRAY_BUT);
    }
    else
      CP_Settings_Fill(GRAY);
    CP_Graphics_DrawRect(2 * baseX, baseY + (baseY * i), buttonWidth, buttonHeight);
    CP_Settings_Fill(WHITE);
    CP_Font_DrawText((first + currentButton)->name, 2 * baseX, baseY + (baseY * i));
    currentButton++;
  }
  if ((worldsScreen  + 1) * 6 < worldsCount) 
  {
    if (checkMouseBoxCollide(SCREEN_WIDTH / 2.0f - 40, 0, 50, 100))
    {
      if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      {
        worldsScreen++;
      }

      CP_Settings_Fill(GRAY_BUT);
    }
    else
      CP_Settings_Fill(GRAY);
    CP_Graphics_DrawRect(SCREEN_WIDTH - 40.0f, SCREEN_HEIGHT / 2.0f, 50, 100);
    CP_Settings_Fill(WHITE);
    CP_Font_DrawText("->", SCREEN_WIDTH - 40.0f, SCREEN_HEIGHT / 2.0f);
  }
  else
  {
    CP_Settings_Fill(GRAY);
    CP_Graphics_DrawRect(SCREEN_WIDTH - 40.0f, SCREEN_HEIGHT / 2.0f, 50, 100);
    CP_Settings_Fill(GRAY_BUT);
    CP_Font_DrawText("->", SCREEN_WIDTH - 40.0f, SCREEN_HEIGHT / 2.0f);
  }

  if (worldsScreen != 0)
  {
    if (checkMouseBoxCollide(-SCREEN_WIDTH / 2.0f + 40, 0, 50, 100))
    {
      if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      {
        worldsScreen--;
      }

      CP_Settings_Fill(GRAY_BUT);
    }
    else
      CP_Settings_Fill(GRAY);
    CP_Graphics_DrawRect(40.0f, SCREEN_HEIGHT / 2.0f, 50, 100);
    CP_Settings_Fill(WHITE);
    CP_Font_DrawText("<-", 40.0f, SCREEN_HEIGHT / 2.0f);
  }
  else 
  {
    CP_Settings_Fill(GRAY);
    CP_Graphics_DrawRect(40.0f, SCREEN_HEIGHT / 2.0f, 50, 100);
    CP_Settings_Fill(GRAY_BUT);
    CP_Font_DrawText("<-", 40.0f, SCREEN_HEIGHT / 2.0f);
  }

  if (checkMouseBoxCollide(0, -(SCREEN_HEIGHT - 40.0f - SCREEN_HEIGHT/2.0f), SCREEN_WIDTH / 4.0f, 50.0f))
  {
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
      inWorlds = false;
      setWorldName(NULL);
      CP_Engine_SetNextGameState(gameLoopInit, gameLoopUpdate, gameLoopExit);
    }

    CP_Settings_Fill(GRAY_BUT);
  }
  else
    CP_Settings_Fill(GRAY);
  CP_Graphics_DrawRect(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 40.0f, SCREEN_WIDTH / 4.0f, 50.0f);
  CP_Settings_Fill(WHITE);
  CP_Font_DrawText("New World", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 40.0f);
}


void setupButtons()
{
  CP_Settings_TextSize(50 * SCREEN_WIDTH / 1920.0f);
  float width = 300 * SCREEN_WIDTH / 1920.0f;
  float height = 100  * (SCREEN_WIDTH / 1920.0f);
  for (int i = 0; i < BUTTONS; i++)
  {
    switch (i)
    {
    case 0:
      snprintf(buttons[i].words, sizeof buttons[i].words, "START!");
      buttons[i].x = (SCREEN_WIDTH / 2.0f);
      buttons[i].y = (SCREEN_HEIGHT / 2.5f);
      buttons[i].width = width;
      buttons[i].height = height;
      break;
    case 1:
      snprintf(buttons[i].words, sizeof buttons[i].words, "OPTIONS!");
      buttons[i].x = (SCREEN_WIDTH / 2.0f);
      buttons[i].y = (SCREEN_HEIGHT / 2.5f) + (height * 1.25f);
      buttons[i].width = width;
      buttons[i].height = height;
      break;
    case 2:
      snprintf(buttons[i].words, sizeof buttons[i].words, "EXIT!");
      buttons[i].x = (SCREEN_WIDTH / 2.0f);
      buttons[i].y = (SCREEN_HEIGHT / 2.5f) + 3 * (height * 1.25f);
      buttons[i].width = width;
      buttons[i].height = height;
      break;
    case 3:
      snprintf(buttons[i].words, sizeof buttons[i].words, "CREDITS!");
      buttons[i].x = (SCREEN_WIDTH / 2.0f);
      buttons[i].y = (SCREEN_HEIGHT / 2.5f) + 2 * (height * 1.25f);
      buttons[i].width = width;
      buttons[i].height = height;
      break;

      
    }
   
  }
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void MainMenuInit(void)
{
  if (getGenerated())
  {
    free(first);
    first = NULL;
  }  

  loadWorldNames();
  initImages();
  setupButtons();
  setGame(false);
  CP_Settings_RectMode(CP_POSITION_CENTER);
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  CP_Settings_EllipseMode(CP_POSITION_CENTER);
  CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_REPEAT);
}

void drawButtons(void) 
{
  for (int i = 0; i < BUTTONS; i++) 
  {
    CP_Color c;
    if (buttons[i].selected == 1) 
    {
      c = GRAY;
    }
    else 
    {
      c = BLACK;
    }
    CP_Settings_Fill(c);
    CP_Graphics_DrawRect(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height);
    drawWords(buttons[i].words, buttons[i].x, buttons[i].y, 50  * (SCREEN_WIDTH / 1920.0f), WHITE);
  }
}

void checkButtons() 
{
  float mX = CP_Input_GetMouseX();
  float mY = CP_Input_GetMouseY();
  for (int i = 0; i < BUTTONS; i++)
  {
    if (mX > buttons[i].x + (.5 * buttons[i].width) || mX < buttons[i].x - (.5 * buttons[i].width) || mY > buttons[i].y + (.5 * buttons[i].height) || mY < buttons[i].y - (.5 * buttons[i].height))
    {
      buttons[i].selected = 0;
    }
    else
    {
      buttons[i].selected = 1;
    }
    if (!buttons[i].selected)
      continue;
    if (!CP_Input_MouseClicked())
      continue;
    switch (i)
    {

    case 0:
      inWorlds = true;
      break;
    case 1:
      CP_Engine_SetNextGameStateForced(OptionsInit, OptionsUpdate, OptionsExit);
      break;
    case 2:
      CP_Engine_Terminate();
      break;
    case 3:
      CP_Engine_SetNextGameState(creditsInit, creditsUpdate, creditsExit);
      break;
    }


  }
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void MainMenuUpdate(void)
{
  CP_Graphics_ClearBackground(CP_Color_Create(117, 117, 117, 255));
  drawBackGroundLayer(&p);
  if (inWorlds) 
  {
    drawWorlds();
    return;
  }
  drawWords("Simply Survive", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 5.0f, 205  * (SCREEN_WIDTH / 1920.0f), BLACK);
  drawButtons();
  float textSize = 70.0f * SCREEN_WIDTH / 1920;
  drawWords("Verson:", SCREEN_WIDTH - 200.0f * textSize / 50, SCREEN_HEIGHT - textSize, textSize, BLACK);
  drawWords(getVersion(), SCREEN_WIDTH - 60.0f * textSize / 50, SCREEN_HEIGHT - textSize, textSize, BLACK);
  checkButtons();
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void MainMenuExit(void)
{

}
