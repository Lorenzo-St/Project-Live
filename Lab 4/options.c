//---------------------------------------------------------
// file: options.c
// author:	Lorenzo St. Luce
// email:	 lorenzo.stluce@digipen.edu
//
// brief:	ile for holding options functions
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "options.h"
#include "mainMenu.h"
#include "structs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define OPTIONSBUTTONS 5
#define AUDIO_SLIDERS 2

button optionsButton[OPTIONSBUTTONS] = { 0 };
button displayScreen[2] = { 0 };
int opsSubScreen = 0;
slider audioSliders[AUDIO_SLIDERS] = { 0 };

void initSliders(void) 
{
  for (int i = 0; i < AUDIO_SLIDERS; i++) 
  {
    switch (i) 
    {
    case 0:
      snprintf(audioSliders[i].title, sizeof audioSliders[i].title, "Sounds");
      audioSliders[i].x = CP_System_GetWindowWidth() / 2.0f;
      audioSliders[i].y = CP_System_GetWindowHeight() / 3.0f;
      audioSliders[i].length = CP_System_GetWindowWidth() / 5.0f;
      audioSliders[i].value = 1.0f;
      break;
    case 1:
      snprintf(audioSliders[i].title, sizeof audioSliders[i].title, "Music");
      audioSliders[i].x = CP_System_GetWindowWidth() / 2.0f;
      audioSliders[i].y = CP_System_GetWindowHeight() / 3.0f + 200;
      audioSliders[i].length = CP_System_GetWindowWidth() / 5.0f;
      audioSliders[i].value = .5f;
      break;
    }
  }
}

void setupOptionsButtons(void)
{
  CP_Settings_TextSize(50);
  for (int i = 0; i < OPTIONSBUTTONS; i++)
  {
    switch (i)
    {
    case 0:
      snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Display!");
      optionsButton[i].x = (CP_System_GetWindowWidth() / 6.0f);
      optionsButton[i].y = (CP_System_GetWindowHeight() / 10.0f);
      optionsButton[i].width = 300;
      optionsButton[i].height = 100;
      break;
    case 1:
      snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Graphics!");
      optionsButton[i].x = (CP_System_GetWindowWidth() / 3.0f);
      optionsButton[i].y = (CP_System_GetWindowHeight() /10.0f);
      optionsButton[i].width = 300;
      optionsButton[i].height = 100;
      break;
    case 2:
      snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Controls!");
      optionsButton[i].x = (CP_System_GetWindowWidth() / 2.0f);
      optionsButton[i].y = (CP_System_GetWindowHeight() / 10.0f);
      optionsButton[i].width = 300;
      optionsButton[i].height = 100;
      break;
    case 3:
      snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Audio!");
      optionsButton[i].x = (CP_System_GetWindowWidth() / 1.5f);
      optionsButton[i].y = (CP_System_GetWindowHeight() / 10.0f);
      optionsButton[i].width = 300;
      optionsButton[i].height = 100;
      break;
    case 4:
      snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Exit!");
      optionsButton[i].x = (CP_System_GetWindowWidth() / 1.2f);
      optionsButton[i].y = (CP_System_GetWindowHeight() / 10.0f);
      optionsButton[i].width = 300;
      optionsButton[i].height = 100;
      break;
      
    }
   
  }
  for (int i = 0; i < 2; i++) 
  {
    switch (i) 
    {
    case 0:
      snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "Set Fullscreen");
      displayScreen[i].x = (CP_System_GetWindowWidth() / 2.0f);
      displayScreen[i].y = (CP_System_GetWindowHeight() / 3.0f);
      displayScreen[i].width = 300;
      displayScreen[i].height = 150;
      break;
    case 1:
      snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "Set Windowed");
      displayScreen[i].x = (CP_System_GetWindowWidth() / 2.0f);
      displayScreen[i].y = (CP_System_GetWindowHeight() / 2.0f);
      displayScreen[i].width = 300;
      displayScreen[i].height = 150;
      break;
    }
  }
}


// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void OptionsInit(void)
{
  initSliders();
  setupOptionsButtons();
  CP_Settings_RectMode(CP_POSITION_CENTER);
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
}

void drawSliders(void) 
{

  for (int i = 0; i < AUDIO_SLIDERS; i++)
  {
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_TextSize(50);
    CP_Font_DrawText(audioSliders[i].title, audioSliders[i].x, audioSliders[i].y - 50);
    CP_Graphics_DrawRect(audioSliders[i].x, audioSliders[i].y, audioSliders[i].length, 20);
    float x = (audioSliders[i].length * audioSliders[i].value);
    if (audioSliders[i].selected == 1)
      CP_Settings_Fill(CP_Color_Create(50, 50, 50, 255));
    else
      CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));

    CP_Graphics_DrawRect(audioSliders[i].x + x - (audioSliders[i].length * .5f), audioSliders[i].y, 20, 30);
  }
}

void checkSliders(void) 
{
  float mX = CP_Input_GetMouseX();
  float mY = CP_Input_GetMouseY();
  for (int i = 0; i < AUDIO_SLIDERS; i++)
  {
    if (mX > audioSliders[i].x + (.5 * audioSliders[i].length) || mX < audioSliders[i].x - (.5 * audioSliders[i].length) || mY > audioSliders[i].y + (10) || mY < audioSliders[i].y - (10))
    {
      audioSliders[i].selected = 0;
    }
    else
    {
      audioSliders[i].selected = 1;
    }
    if (audioSliders[i].selected)
    {
      if (CP_Input_MouseDown(0))
      {
        float halfLength = audioSliders[i].length / 2.0f;
        float devisor, divedend;
        devisor = (mX - (audioSliders[i].x - halfLength));
        divedend = (((audioSliders[i].x + halfLength)) - (audioSliders[i].x - halfLength));
        audioSliders[i].value = devisor / divedend;
        switch (i) 
        {
        case 0:
          CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, audioSliders[i].value);
          break;
        case 1:
          CP_Sound_SetGroupVolume(CP_SOUND_GROUP_1, audioSliders[i].value);
          break;
        }
      }
    }
  }
}

void drawOptionsButtons(void) 
{
  for (int i = 0; i < OPTIONSBUTTONS; i++)
  {
    if (optionsButton[i].selected == 1) 
    {
      CP_Settings_Fill(CP_Color_Create(255, 100, 255, 255));
      CP_Graphics_DrawRect(optionsButton[i].x, optionsButton[i].y, optionsButton[i].width, optionsButton[i].height);
      CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
      CP_Font_DrawText(optionsButton[i].words, optionsButton[i].x, optionsButton[i].y);
    }
    else 
    {
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Graphics_DrawRect(optionsButton[i].x, optionsButton[i].y, optionsButton[i].width, optionsButton[i].height);
      CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
      CP_Font_DrawText(optionsButton[i].words, optionsButton[i].x, optionsButton[i].y);
    }
  }
}

void checkOptionsButtons(void) 
{
  float mX = CP_Input_GetMouseX();
  float mY = CP_Input_GetMouseY();
  for (int i = 0; i < OPTIONSBUTTONS; i++)
  {
    if (mX > optionsButton[i].x + (.5 * optionsButton[i].width) || mX < optionsButton[i].x - (.5 * optionsButton[i].width) || mY > optionsButton[i].y + (.5 * optionsButton[i].height) || mY < optionsButton[i].y - (.5 * optionsButton[i].height))
    {
      optionsButton[i].selected = 0;
    }
    else 
    {
      optionsButton[i].selected = 1;
    }
    if (optionsButton[i].selected) 
    {
      if (CP_Input_MouseClicked()) 
      {
        switch (i) 
        {

        case 0:
          opsSubScreen = 0;
          break;
        case 1:
          opsSubScreen = 1;
          break;
        case 2:
          opsSubScreen = 2;
          break;
        case 3:
          opsSubScreen = 3;
          break;
        case 4:
          CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
          break;
        }
      }
    }
  }
}

void drawSubScreen(void)
{
  float mX = CP_Input_GetMouseX();
  float mY = CP_Input_GetMouseY();
  char buffer[1000] = { 0 };
  switch (opsSubScreen)
  {
  case 0:
    snprintf(buffer, sizeof buffer, "The current screen resolution is %i by %i", CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
    CP_Font_DrawText(buffer, CP_System_GetWindowWidth() / 2.0f, 250.0f);
    for (int i = 0; i < 2; i++)
    {
      if (mX > displayScreen[i].x + (.5 * displayScreen[i].width) || mX < displayScreen[i].x - (.5 * displayScreen[i].width) || mY > displayScreen[i].y + (.5 * displayScreen[i].height) || mY < displayScreen[i].y - (.5 * displayScreen[i].height))
      {
        displayScreen[i].selected = 0;
      }
      else
      {
        displayScreen[i].selected = 1;
      }
      if (displayScreen[i].selected == 1)
      {
        CP_Settings_Fill(CP_Color_Create(255, 100, 255, 255));
        CP_Graphics_DrawRect(displayScreen[i].x, displayScreen[i].y, displayScreen[i].width, displayScreen[i].height);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawText(displayScreen[i].words, displayScreen[i].x, displayScreen[i].y);
      }
      else
      {
        CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
        CP_Graphics_DrawRect(displayScreen[i].x, displayScreen[i].y, displayScreen[i].width, displayScreen[i].height);
        CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
        CP_Font_DrawText(displayScreen[i].words, displayScreen[i].x, displayScreen[i].y);
      }
      if (displayScreen[i].selected) 
      {
        if (CP_Input_MouseClicked())
        {
          switch (i) 
          {
          case 0:
            CP_System_FullscreenAdvanced(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
            setupOptionsButtons();
            break;
          case 1:
            CP_System_SetWindowSize(1920, 1080);
            setupOptionsButtons();
            break;
          }
        }
      }
    }
    break;
  case 1:
    snprintf(buffer, sizeof buffer, "Wumpus is very lonely today");
    CP_Font_DrawText(buffer, CP_System_GetWindowWidth() / 2.0f, 250.0f);
    break;
  case 2:;
    float xpos = CP_System_GetWindowWidth() / 3.0f;
    float ypos = CP_System_GetWindowHeight() / 3.0f;
    CP_Settings_Fill(CP_Color_Create(50, 50, 50, 255));
    CP_Graphics_DrawRect(xpos, ypos, CP_System_GetWindowWidth() / 35.0f, CP_System_GetWindowHeight() / 20.0f);
    CP_Graphics_DrawRect(xpos, ypos + 100, CP_System_GetWindowWidth() / 35.0f, CP_System_GetWindowHeight() / 20.0f);
    CP_Graphics_DrawRect(xpos, ypos + 200, CP_System_GetWindowWidth() / 35.0f, CP_System_GetWindowHeight() / 20.0f);
    CP_Graphics_DrawRect(xpos, ypos + 300, CP_System_GetWindowWidth() / 35.0f, CP_System_GetWindowHeight() / 20.0f);
    CP_Graphics_DrawRect(xpos, ypos + 400, CP_System_GetWindowWidth() / 15.0f, CP_System_GetWindowHeight() / 20.0f);
    CP_Graphics_DrawEllipse(xpos - 150, ypos + 400, 100, 150);
    CP_Graphics_DrawEllipse(xpos, ypos + 550, 100, 150);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawEllipse(xpos - 150, ypos + 357, 20, 50);
    CP_Graphics_DrawEllipse(xpos, ypos + 507, 20, 50);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Graphics_DrawEllipse(xpos - 175, ypos + 357, 30, 30);
    CP_Font_DrawText("W", xpos, ypos);
    CP_Font_DrawText("Move up", xpos + 300, ypos);
    CP_Font_DrawText("A", xpos, ypos + 100);
    CP_Font_DrawText("Move left", xpos + 300, ypos + 100);
    CP_Font_DrawText("S", xpos, ypos + 200);
    CP_Font_DrawText("Move down", xpos + 300, ypos + 200);
    CP_Font_DrawText("D", xpos, ypos + 300);
    CP_Font_DrawText("Move right", xpos + 300, ypos + 300);
    CP_Font_DrawText("Space", xpos, ypos + 400);
    CP_Font_DrawText("Shoot", xpos + 300, ypos + 400);
    CP_Font_DrawText("Aim", xpos + 300, ypos + 550);
    CP_Settings_TextSize(20);
    CP_Font_DrawText("Left Mouse", xpos - 150, ypos + 400);
    break;
  case 3:
    drawSliders();
    break;
  }
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void OptionsUpdate(void)
{
  CP_Settings_TextSize(40);
  CP_Graphics_ClearBackground(CP_Color_CreateHex(0x111111ff));
  drawOptionsButtons();
  checkSliders();
  checkOptionsButtons();
  drawSubScreen();

}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void OptionsExit(void)
{

}
