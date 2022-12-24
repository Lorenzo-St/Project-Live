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
#include "endScreen.h"
#include "globalData.h"
#include "scoring.h"
#include "mainMenu.h"
#include "structs.h"
#include "gameLoop.h"
#include "stdio.h"

#define BUTTONS 3

button endButtons[BUTTONS] = { 0 };

void setupEndButtons()
{
  CP_Settings_TextSize(50);
  for (int i = 0; i < BUTTONS; i++)
  {
    switch (i)
    {
    case 0:
      snprintf(endButtons[i].words, sizeof endButtons[i].words, "YES!");
      endButtons[i].x = (SCREEN_WIDTH / 3.0f);
      endButtons[i].y = (SCREEN_HEIGHT / 1.5f);
      endButtons[i].width = 300;
      endButtons[i].height = 100;
      break;
    case 1:
      snprintf(endButtons[i].words, sizeof endButtons[i].words, "NO!");
      endButtons[i].x = (SCREEN_WIDTH / 1.5f);
      endButtons[i].y = (SCREEN_HEIGHT / 1.5f);
      endButtons[i].width = 300;
      endButtons[i].height = 100;
      break;
    case 2:
      snprintf(endButtons[i].words, sizeof endButtons[i].words, "Save & Exit!");
      endButtons[i].x = (SCREEN_WIDTH / 2.0f);
      endButtons[i].y = (SCREEN_HEIGHT / 1.25f);
      endButtons[i].width = 300;
      endButtons[i].height = 100;
      break;
    }
  }
}
void checkEndButtons()
{
  float mX = CP_Input_GetMouseX();
  float mY = CP_Input_GetMouseY();
  for (int i = 0; i < BUTTONS; i++)
  {
    if (mX > endButtons[i].x + (.5 * endButtons[i].width) || mX < endButtons[i].x - (.5 * endButtons[i].width) || mY > endButtons[i].y + (.5 * endButtons[i].height) || mY < endButtons[i].y - (.5 * endButtons[i].height))
    {
      endButtons[i].selected = 0;
    }
    else
    {
      endButtons[i].selected = 1;
    }
    if (endButtons[i].selected)
    {
      if (CP_Input_MouseClicked())
      {
        switch (i)
        {

        case 0:
          CP_Engine_SetNextGameState(gameLoopInit, gameLoopUpdate, gameLoopExit);
          break;
        case 1:
          CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
          break;
        case 2:
          CP_Engine_SetNextGameState(scoresInit, scoresUpdate, scoresExit);
          break;
        }
      }
    }
  }
}
void drawEndButtons()
{
  for (int i = 0; i < BUTTONS; i++)
  {
    if (endButtons[i].selected == 1)
    {
      CP_Settings_Fill(CP_Color_Create(255, 100, 255, 255));
      CP_Graphics_DrawRect(endButtons[i].x, endButtons[i].y, endButtons[i].width, endButtons[i].height);
      CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
      CP_Font_DrawText(endButtons[i].words, endButtons[i].x, endButtons[i].y);
    }
    else
    {
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Graphics_DrawRect(endButtons[i].x, endButtons[i].y, endButtons[i].width, endButtons[i].height);
      CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
      CP_Font_DrawText(endButtons[i].words, endButtons[i].x, endButtons[i].y);
    }
  }
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void endScreenInit(void)
{
  setupEndButtons();
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void endScreenUpdate(void)
{
  CP_Graphics_ClearBackground(CP_Color_Create(139, 50, 77, 100));
  CP_Settings_TextSize(70);
  CP_Settings_Fill(CP_Color_Create(10, 10, 10, 255));
  CP_Font_DrawText("YOU DIED", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f);
  CP_Font_DrawText("PLAY AGAIN?", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100);

  char buff[100];
  snprintf(buff, sizeof buff, "%.3f seconds", getTime());
  CP_Font_DrawText("You lasted for:", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f - 80);
  CP_Font_DrawText(buff, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f);
  CP_Settings_TextSize(50);
  CP_Settings_StrokeWeight(10);
  drawEndButtons();
  checkEndButtons();
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void endScreenExit(void)
{

}
