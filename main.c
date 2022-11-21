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
#define KEY_COUNT 1

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int playerInput[KEY_COUNT] = { 256 };

void preUpdate(void);

int main(void)
{
  CP_System_SetFrameRate(60.0f);
	CP_Engine_SetPreUpdateFunction(preUpdate);
	CP_Engine_SetNextGameState(SplashInit, SplashUpdate, SplashExit);
	CP_Engine_Run();
	return 0;
}
void preUpdate()
{
  for (int i = 0; i < KEY_COUNT; i++)
  {
    if (CP_Input_KeyTriggered(playerInput[i]))
    {
      switch (playerInput[i])
      {
      case 256:
        CP_Engine_Terminate();
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
