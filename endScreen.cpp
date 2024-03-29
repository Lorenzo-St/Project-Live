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
#include "playerInput.h"
extern "C"
{
#include "cprocessing.h"
#include "endScreen.h"
#include "globalData.h"
#include "scoring.h"
#include "mainMenu.h"
#include "structs.h"
#include "gameLoop.h"
#include "stdio.h"
#define BUTTONS 2

  button endButtons[BUTTONS] = { 0 };

  void setupEndButtons()
  {
    CP_Settings_TextSize(50 * SCREEN_WIDTH / 1920.0f);
    float width = 300 * SCREEN_WIDTH / 1920.0f;
    float height = 100 * SCREEN_WIDTH / 1920.0f;
    for (int i = 0; i < BUTTONS; i++)
    {
      switch (i)
      {
      case 0:
        snprintf(endButtons[i].words, sizeof endButtons[i].words, "Restart!");
        endButtons[i].x = (SCREEN_WIDTH / 2.0f);
        endButtons[i].y = (SCREEN_HEIGHT / 1.5f);
        endButtons[i].width = width;
        endButtons[i].height = height;
        break;
      case 1:
        snprintf(endButtons[i].words, sizeof endButtons[i].words, "Quit!");
        endButtons[i].x = (SCREEN_WIDTH / 2.0f);
        endButtons[i].y = (SCREEN_HEIGHT / 1.25f);
        endButtons[i].width = width;
        endButtons[i].height = height;
        break;
      }
    }
  }
  void checkEndButtons()
  {
    for (int i = 0; i < BUTTONS; ++i)
      endButtons[i].selected = false;
    float mX = CP_Input_GetMouseX();
    float mY = CP_Input_GetMouseY();
    static int selected = 0;
    for (int i = 0; i < BUTTONS; i++)
    {
      if (mX > endButtons[i].x + (.5 * endButtons[i].width) || mX < endButtons[i].x - (.5 * endButtons[i].width) || mY > endButtons[i].y + (.5 * endButtons[i].height) || mY < endButtons[i].y - (.5 * endButtons[i].height))
      {
        ;
      }
      else
        selected = i;

    }

    if (isTriggered(YAxisPos))
    {
      --selected;
    }
    else if (isTriggered(YAxisNeg))
    {
      ++selected;
    }
    if (selected >= BUTTONS)
      selected = 0;
    if (selected < 0)
      selected = BUTTONS - 1;
    endButtons[selected].selected = true;
    if (isTriggered(Confirm))
    {
      switch (selected)
      {

      case 0:
        CP_Engine_SetNextGameState(gameLoopInit, gameLoopUpdate, gameLoopExit);
        break;
      case 1:
        CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
        break;
      }
    }
  }
  void drawEndButtons()
  {
    for (int i = 0; i < BUTTONS; i++)
    {
      if (endButtons[i].selected == 1)
      {
        CP_Settings_Fill(GRAY);
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
    setGame(false);
    setupEndButtons();
  }

  // use CP_Engine_SetNextGameState to specify this function as the update function
  // this function will be called repeatedly every frame
  void endScreenUpdate(void)
  {
    CP_Graphics_ClearBackground(CP_Color_Create(139, 50, 77, 100));
    CP_Settings_TextSize(70 * SCREEN_WIDTH / 1920.0f);
    CP_Settings_Fill(CP_Color_Create(10, 10, 10, 255));
    CP_Font_DrawText("YOU DIED", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f);
    CP_Font_DrawText("PLAY AGAIN?", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100 * SCREEN_WIDTH / 1920.0f);

    char buff[100];
    snprintf(buff, sizeof buff, "%.3f seconds", getTime());
    CP_Font_DrawText("You lasted for:", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f - 80 * SCREEN_WIDTH / 1920.0f);
    CP_Font_DrawText(buff, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 4.0f);
    CP_Settings_TextSize(50 * SCREEN_WIDTH / 1920.0f);
    CP_Settings_StrokeWeight(10 * SCREEN_WIDTH / 1920.0f);
    drawEndButtons();
    checkEndButtons();
  }

  // use CP_Engine_SetNextGameState to specify this function as the exit function
  // this function will be called once just before leaving the current gamestate
  void endScreenExit(void)
  {

  }
}
