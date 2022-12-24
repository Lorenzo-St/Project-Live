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
#include "structs.h"
#include "mainMenu.h"
#include <stdio.h>

button returnButton;

void initButton(void) 
{
	returnButton.height = 100;
	returnButton.width = 300;
	returnButton.x = SCREEN_WIDTH / 2.0f;
	returnButton.y = SCREEN_HEIGHT / 1.25f;
	snprintf(returnButton.words, sizeof returnButton.words, "Return");
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void creditsInit(void)
{

}

void drawButton(void)
{
  if (returnButton.selected == 1)
  {
    CP_Settings_Fill(CP_Color_Create(255, 100, 255, 255));
    CP_Graphics_DrawRect(returnButton.x, returnButton.y, returnButton.width, returnButton.height);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(returnButton.words, returnButton.x, returnButton.y);
  }
  else
  {
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRect(returnButton.x, returnButton.y, returnButton.width, returnButton.height);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(returnButton.words, returnButton.x, returnButton.y);
  }

}

void checkButton(void)
{
  float mX = CP_Input_GetMouseX();
  float mY = CP_Input_GetMouseY();

  if (mX > returnButton.x + (.5 * returnButton.width) || mX < returnButton.x - (.5 * returnButton.width) || mY > returnButton.y + (.5 * returnButton.height) || mY < returnButton.y - (.5 * returnButton.height))
  {
    returnButton.selected = 0;
  }
  else
  {
    returnButton.selected = 1;
  }
  if (returnButton.selected)
  {
    if (CP_Input_MouseClicked())
    {
      CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
    }
  }

}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void creditsUpdate(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));
  drawButton();
	CP_Settings_TextSize(40);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Create by: Lorenzo St. Luce", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f);
	CP_Font_DrawText("Instructor: Andy Ellinger", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.5f);
	CP_Font_DrawText("Special Thanks: My roommates for play testing.", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	CP_Font_DrawText("Copyright (c) 2020 DigiPen, All rights reserved.", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.75f);
  checkButton();

}



// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void creditsExit(void)
{

}
