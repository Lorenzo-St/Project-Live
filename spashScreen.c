#include "cprocessing.h"
#include "spashScreen.h"
#include "mainMenu.h"
#include "structs.h"
#include "globalData.h"
#include "Sound.h"
#include <stdio.h>

static CP_Image logo;
static int alpha = 255;
static float counter;
static float angle = 0;
static float imageX = 0;
static float imageY = 0;
static float imageWidth = 0;
static float imageHeight = 0;

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void SplashInit(void)
{
	initSounds();

	//CP_System_FullscreenAdvanced(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
	CP_System_SetWindowSize(WINDOW_SIZE);
	char buffer[40] = { 0 };
	snprintf(buffer, sizeof buffer, "Simply Survive v%s", getVersion());
	CP_System_SetWindowTitle(buffer);
	logo = CP_Image_Load("./Assets/FMOD Logo Black - White Background.png");
	CP_Graphics_ClearBackground(WHITE);
	imageX = SCREEN_WIDTH / 2.0f;
	imageY = SCREEN_HEIGHT / 2.0f;
	imageHeight = (float)CP_Image_GetHeight(logo) * SCREEN_WIDTH / 1920.0f;
	imageWidth = (float)CP_Image_GetWidth(logo) * SCREEN_WIDTH / 1920.0f;
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void SplashUpdate(void)
{
	if (counter > 4)
	{
		alpha -= 5;	
		if (alpha < -10) 
			CP_Engine_SetNextGameStateForced(MainMenuInit, MainMenuUpdate, MainMenuExit);
	}
	if (CP_Input_KeyTriggered(KEY_ANY) || CP_Input_MouseClicked())
	{
		counter = 100;
		alpha = -100;
	}

	CP_Graphics_ClearBackground(WHITE);

	CP_Image_DrawAdvanced(logo, imageX, imageY, imageWidth, imageHeight, alpha, angle);
	counter += CP_System_GetDt();

}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void SplashExit(void)
{
	CP_Image_Free(&logo);
}
