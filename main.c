//---------------------------------------------------------
// file:	main.c
// author:	Lorenzo St. Luce	
// email:	 lorenzo.stluce@digipen.edu
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "spashScreen.h"
#include "globalData.h"
#include "structs.h"
#include "drawStuff.h"
#include "checkStuff.h"
#include "options.h"
#include "mainMenu.h"

#define KEY_COUNTS 1

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int playerInput[KEY_COUNTS] = { KEY_ESCAPE };

void preUpdate(void)
{
  for (int i = 0; i < KEY_COUNT; i++)
  {
    if (CP_Input_KeyTriggered(playerInput[i]))
    {
      switch (playerInput[i])
      {
      case KEY_ESCAPE:
        if(getGame())
          setPause(!getPause());
        break;
      }
    }
    if (CP_Input_KeyDown(playerInput[i]))
    {
      switch (playerInput[i])
      {


      }
    }
  }
}

void postUpdate(void) 
{
  if (!getPause())
    return;
  CP_Color C = CP_Color_CreateHex(0x8498A7A0);
  CP_Settings_Fill(C);
  CP_Graphics_DrawRectAdvanced(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f, 0, 0);

  C = BLACK;
  CP_Settings_Stroke(C);
  drawWords("Game is Paused!!", SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 10.0f, 50 * SCREEN_WIDTH / 1920.0f, BLACK);

  float x = SCREEN_WIDTH / 2.0f;
  float y = SCREEN_HEIGHT / 3.0f;
  float width = SCREEN_WIDTH / 7.0f;
  float height = SCREEN_HEIGHT / 10.0f;
  if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), width, height))
  {
    C = GRAY;
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      setPause(false);
  } 
  else
   C = WHITE;
  CP_Settings_Fill(C);
  CP_Graphics_DrawRect(x, y, width, height);
  drawWords("Resume!", x, y, 40, BLACK);

  y += height * 1.15f;
  if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), width, height))
  {
    C = GRAY;
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      CP_Engine_SetNextGameStateForced(OptionsInit, OptionsUpdate, OptionsExit);
  }
  else
    C = WHITE;
  CP_Settings_Fill(C);
  CP_Graphics_DrawRect(x, y, width, height);
  drawWords("Options!", x, y, 40, BLACK);

  y += height * 1.15f;
  if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), width, height))
  {
    C = GRAY;
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
      CP_Engine_SetNextGameStateForced(MainMenuInit, MainMenuUpdate, MainMenuExit);
      setPause(false);
    }

  }
  else
    C = WHITE;
  CP_Settings_Fill(C);
  CP_Graphics_DrawRect(x, y, width, height);
  drawWords("Main Menu!", x, y, 40, BLACK);

}

int main(void)
{
  CP_System_SetFrameRate(60.0f);
	CP_Engine_SetPreUpdateFunction(preUpdate);
  CP_Engine_SetPostUpdateFunction(postUpdate);
	CP_Engine_SetNextGameState(SplashInit, SplashUpdate, SplashExit);
	CP_Engine_Run();
	return 0;
}

