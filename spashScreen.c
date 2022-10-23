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
#include "spashScreen.h"
#include "mainMenu.h"

CP_Image logo;
int alpha = 255;
float counter;
float angle = 0;
float imageX = 0;
float imageY = 0;
float imageWidth = 0;
float imageHeight = 0;
// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void SplashInit(void)
{
	//CP_System_FullscreenAdvanced(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
	CP_System_SetWindowSize(1920, 1080);
	CP_System_SetWindowTitle("Live to Survive");
	logo = CP_Image_Load("./Assets/DigiPen_WHITE.png");
	CP_Graphics_ClearBackground(CP_Color_CreateHex(0x000000));
	imageX = CP_System_GetWindowWidth() / 2.0f;
	imageY = CP_System_GetWindowHeight() / 2.0f;
	imageHeight = (float)CP_Image_GetHeight(logo);
	imageWidth = (float)CP_Image_GetWidth(logo);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void SplashUpdate(void)
{
	if (counter > 4)
	{
		alpha -= 5;
		imageHeight -= 5;
		imageWidth -= 5;
		if (imageWidth <= 0) imageWidth = 0;
		if (imageHeight <= 0) imageHeight = 0;
		imageY += 10;
		if (alpha < -10) 
			CP_Engine_SetNextGameStateForced(MainMenuInit, MainMenuUpdate, MainMenuExit);
	}
	if (CP_Input_KeyTriggered(KEY_ANY) || CP_Input_MouseClicked())
	{
		counter = 100;
		alpha = -100;
	}

	CP_Graphics_ClearBackground(CP_Color_CreateHex(0x000000));

	CP_Image_DrawAdvanced(logo, imageX, imageY, imageWidth, imageHeight, alpha, angle);
	counter += CP_System_GetDt();

}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void SplashExit(void)
{

}
