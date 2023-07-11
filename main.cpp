extern "C" 
{

#include "cprocessing.h"
#include "spashScreen.h"
#include "globalData.h"
#include "structs.h"
#include "drawStuff.h"
#include "checkStuff.h"
#include "options.h"
#include "mainMenu.h"
#include "Sound.h"
#include "profileData.h"
}
#include "playerInput.h"

#define KEY_COUNTS 1

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int playerInput[KEY_COUNTS] = { KEY_ESCAPE };



void preUpdate(void)
{


  if (isTriggered(Back) && getPause()) 
  {
    if (getGame() && !getOptions())
      setPause(false);
    return;
  }
  if (isTriggered(Pause) && !getPause()) 
  {
    if (getGame() && !getOptions())
      setPause(true);
    return;
  }
}

void postUpdate(void)
{

  if (!getPause())
    return;
  static int selected = 0;
  CP_Color C = CP_Color_CreateHex(0x8498A7A0);
  switch (GetMode())
  {
  case LightMode:
    C = CP_Color_CreateHex(0x8498A7A0);
    break;
  case DarkMode:
    C = CP_Color_CreateHex(0x263E7FA0);
    break;
  }
  CP_Settings_Fill(C);
  CP_Graphics_DrawRectAdvanced(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f, 0, 0);

  C = BLACK;
  CP_Settings_Stroke(C);
  drawWords("Game is Paused!!", SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 10.0f, 50 * SCREEN_WIDTH / 1920.0f, BLACK);

  float x = SCREEN_WIDTH / 2.0f;
  float y = SCREEN_HEIGHT / 3.0f;
  float width = SCREEN_WIDTH / 7.0f;
  float height = SCREEN_HEIGHT / 10.0f;
#define BUTTONS 3
  const char* names[3] = { "Resume!" , "Options!" , "Main Menu!" };
  for (int i = 0; i < BUTTONS; ++i) 
  {
    if (checkControllerConectivity() == false)
    {

      if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), width, height))
      {
        selected = i;
      }
    }
    
    if(selected == i)
      C = GRAY;
    else
      C = WHITE;
    CP_Settings_Fill(C);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords(names[i], x, y, 40 * (SCREEN_WIDTH / 1920.0f), BLACK);
    y += height * 1.15f;

  }
  if (isTriggered(YAxisPos) && selected != 0)
  {
    --selected;
  }
  else if (isTriggered(YAxisNeg) && selected != BUTTONS - 1)
  {
    ++selected;
  }
  if (isTriggered(Confirm)) 
  {
    switch (selected)
    {
    case 0:
      setPause(false);
      break;
    case 1:
      CP_Engine_SetNextGameStateForced(OptionsInit, OptionsUpdate, OptionsExit);
      setOptions(true);
      break;
    case 2:
      setGame(false);
      CP_Engine_SetNextGameStateForced(MainMenuInit, MainMenuUpdate, MainMenuExit);
      setPause(false);
      break;
    
    }
  }


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

