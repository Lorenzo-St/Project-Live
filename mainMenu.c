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
#include <stdio.h>
#include <string.h>
#include <math.h>

#define BUTTONS 4

button buttons[BUTTONS] = { 0 };

void setupButtons()
{
  CP_Settings_TextSize(50);
  for (int i = 0; i < BUTTONS; i++)
  {
    switch (i)
    {
    case 0:
      snprintf(buttons[i].words, sizeof buttons[i].words, "START!");
      buttons[i].x = (SCREEN_WIDTH / 2.0f);
      buttons[i].y = (SCREEN_HEIGHT / 2.5f);
      buttons[i].width = 300;
      buttons[i].height = 100;
      break;
    case 1:
      snprintf(buttons[i].words, sizeof buttons[i].words, "OPTIONS!");
      buttons[i].x = (SCREEN_WIDTH / 2.0f);
      buttons[i].y = (SCREEN_HEIGHT /2.0f);
      buttons[i].width = 300;
      buttons[i].height = 100;
      break;
    case 2:
      snprintf(buttons[i].words, sizeof buttons[i].words, "EXIT!");
      buttons[i].x = (SCREEN_WIDTH / 2.0f);
      buttons[i].y = (SCREEN_HEIGHT / 1.4f);
      buttons[i].width = 300;
      buttons[i].height = 100;
      break;
    case 3:
      snprintf(buttons[i].words, sizeof buttons[i].words, "CREDITS!");
      buttons[i].x = (SCREEN_WIDTH / 2.0f);
      buttons[i].y = (SCREEN_HEIGHT / 1.65f);
      buttons[i].width = 300;
      buttons[i].height = 100;
      break;

      
    }
   
  }
}


// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void MainMenuInit(void)
{
  setupButtons();
  CP_Settings_RectMode(CP_POSITION_CENTER);
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
}

void drawButtons(void) 
{
  for (int i = 0; i < BUTTONS; i++) 
  {
    if (buttons[i].selected == 1) 
    {
      CP_Settings_Fill(CP_Color_Create(255, 100, 255, 255));
      CP_Graphics_DrawRect(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height);
      CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
      CP_Font_DrawText(buttons[i].words, buttons[i].x, buttons[i].y);
    }
    else 
    {
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Graphics_DrawRect(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height);
      CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
      CP_Font_DrawText(buttons[i].words, buttons[i].x, buttons[i].y);
    }
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
    if (buttons[i].selected) 
    {
      if (CP_Input_MouseClicked()) 
      {
        switch (i) 
        {

        case 0:
          CP_Engine_SetNextGameStateForced(gameLoopInit, gameLoopUpdate, gameLoopExit);
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
  }
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void MainMenuUpdate(void)
{
  CP_Graphics_ClearBackground(CP_Color_Create(117, 117, 117, 255));
  CP_Settings_TextSize(205);
  CP_Settings_Fill(CP_Color_Create(250, 250, 250, 255));
  CP_Font_DrawText("LIVE TO SURVIVE", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 5.0f);
  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
  CP_Settings_TextSize(200);
  CP_Font_DrawText("LIVE TO SURVIVE", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 5.0f);
  CP_Settings_TextSize(50);
  drawButtons();

  checkButtons();
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void MainMenuExit(void)
{

}
