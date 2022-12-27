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
#include "drawStuff.h"

#include <stdio.h>

button returnButton;

void initButton(void) 
{
	returnButton.height = 100;
	returnButton.width = 300;
	returnButton.x = SCREEN_WIDTH / 2.0f;
	returnButton.y = SCREEN_HEIGHT  * .9f;
	snprintf(returnButton.words, sizeof returnButton.words, "Return");
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void creditsInit(void)
{
  initButton();
}

void drawButton(void)
{
  CP_Color c;
  if (returnButton.selected == 1)
  {
    c = GRAY;
  }
  else
  {
    c = BLACK;
  }
  CP_Settings_Fill(c);
  CP_Graphics_DrawRect(returnButton.x, returnButton.y, returnButton.width, returnButton.height);
  CP_Settings_Fill(WHITE);
  CP_Font_DrawText(returnButton.words, returnButton.x, returnButton.y);
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

void drawCredits(void) 
{
  drawWords("Descent Studios", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 7.0f, 100 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Programming:", SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 3.0f, 60 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Graphics:", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f, 60 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Producing:", SCREEN_WIDTH / 4.0f * 3.0f, SCREEN_HEIGHT / 3.0f, 60 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Lorenzo", SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 3.0f + 40 * SCREEN_WIDTH / 1920.0f, 40 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Lorenzo", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f + 40 * SCREEN_WIDTH / 1920.0f, 40 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Lorenzo", SCREEN_WIDTH / 4.0f * 3.0f, SCREEN_HEIGHT / 3.0f + 40 * SCREEN_WIDTH / 1920.0f, 40 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Play Testing:", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 60 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Carter", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 50 * SCREEN_WIDTH / 1920.0f, 40 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Yakshin", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 100 * SCREEN_WIDTH / 1920.0f, 40 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Logan", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 150 * SCREEN_WIDTH / 1920.0f, 40 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Phillip", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 200 * SCREEN_WIDTH / 1920.0f, 40 * SCREEN_WIDTH / 1920.0f, BLACK);
  drawWords("Jackson", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 250 * SCREEN_WIDTH / 1920.0f, 40 * SCREEN_WIDTH / 1920.0f, BLACK);

  drawWords("Made with FMOD Studio by Firelight Technologies Pty Ltd.", SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * .8f, 50, BLACK);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void creditsUpdate(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));
  drawButton();
	CP_Settings_TextSize(40);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  drawCredits();
  checkButton();

}



// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void creditsExit(void)
{

}
