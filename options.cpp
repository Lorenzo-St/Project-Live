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

#include "playerInput.h"
#include <string>
#include <array>
extern "C" {
#include "cprocessing.h"
#include "options.h"
#include "mainMenu.h"
#include "structs.h"
#include "drawStuff.h"
#include "gameLoop.h"
#include "globalData.h"
#include "profileData.h"

#include <stdio.h>
#include <string.h>
#include <math.h>


#define OPTIONSBUTTONS 5
#define AUDIO_SLIDERS 2
#define DISPLASY_BUTTONS 7

  static std::array<button, OPTIONSBUTTONS>  optionsButton = { 0 };
  static std::array<button, DISPLASY_BUTTONS> displayScreen = { 0 };
  static int opsSubScreen = 0;
  static std::array<slider, AUDIO_SLIDERS> audioSliders = { 0 };
  static bool InnerSelected = false;
  static int* screenSelections;
  static bool enteredFrame;
  void initSliders(void)
  {
    for (int i = 0; i < AUDIO_SLIDERS; i++)
    {
      switch (i)
      {
      case 0:
        snprintf(audioSliders[i].title, sizeof audioSliders[i].title, "Sounds");
        audioSliders[i].x = SCREEN_WIDTH / 2.0f;
        audioSliders[i].y = SCREEN_HEIGHT / 3.0f;
        audioSliders[i].length = SCREEN_WIDTH / 5.0f;
        audioSliders[i].value = GetSounds();
        CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, audioSliders[i].value);

        break;
      case 1:
        snprintf(audioSliders[i].title, sizeof audioSliders[i].title, "Music");
        audioSliders[i].x = SCREEN_WIDTH / 2.0f;
        audioSliders[i].y = SCREEN_HEIGHT / 3.0f + 200;
        audioSliders[i].length = SCREEN_WIDTH / 5.0f;
        audioSliders[i].value = GetMusic();
        CP_Sound_SetGroupVolume(CP_SOUND_GROUP_1, audioSliders[i].value);

        break;
      }
    }
  }

  void setupOptionsButtons(void)
  {
    CP_Settings_TextSize(50);

    float width = 300.0f * SCREEN_WIDTH / 1920.0f;
    float height = 100.0f * (SCREEN_WIDTH / 1920.0f);

    for (int i = 0; i < OPTIONSBUTTONS; i++)
    {
      switch (i)
      {
      case 0:
        snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Display!");
        optionsButton[i].x = (SCREEN_WIDTH / 4.5f);
        optionsButton[i].y = (SCREEN_HEIGHT / 10.0f);
        optionsButton[i].width = width;
        optionsButton[i].height = height;
        break;
      case 1:
        snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Controls!");
        optionsButton[i].x = (SCREEN_WIDTH / 4.5f) + (width * 1.16f);
        optionsButton[i].y = (SCREEN_HEIGHT / 10.0f);
        optionsButton[i].width = width;
        optionsButton[i].height = height;
        break;
      case 2:
        snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Audio!");
        optionsButton[i].x = (SCREEN_WIDTH / 4.5f) + 2 * (width * 1.16f);
        optionsButton[i].y = (SCREEN_HEIGHT / 10.0f);
        optionsButton[i].width = width;
        optionsButton[i].height = height;
        break;
      case 3:
        snprintf(optionsButton[i].words, sizeof optionsButton[i].words, "Exit!");
        optionsButton[i].x = (SCREEN_WIDTH / 4.5f) + 3 * (width * 1.16f);
        optionsButton[i].y = (SCREEN_HEIGHT / 10.0f);
        optionsButton[i].width = width;
        optionsButton[i].height = height;
        break;

      }

    }
    width = 250.0f * SCREEN_WIDTH / 1920.0f;
    for (int i = 0; i < DISPLASY_BUTTONS; i++)
    {
      switch (i)
      {
      case 0:
        snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "Fullscreen");
        displayScreen[i].x = (SCREEN_WIDTH / 3.0f);
        displayScreen[i].y = (SCREEN_HEIGHT / 3.0f);
        displayScreen[i].width = width;
        displayScreen[i].height = height;
        break;
      case 1:
        snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "1920 x 1080");
        displayScreen[i].x = (SCREEN_WIDTH / 3.0f);
        displayScreen[i].y = (SCREEN_HEIGHT / 3.0f) + (height * 1.10f);
        displayScreen[i].width = width;
        displayScreen[i].height = height;
        break;
      case 2:
        snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "1280 x 720");
        displayScreen[i].x = (SCREEN_WIDTH / 3.0f);
        displayScreen[i].y = (SCREEN_HEIGHT / 3.0f) + 2 * (height * 1.10f);
        displayScreen[i].width = width;
        displayScreen[i].height = height;
        break;
      case 3:
        snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "800 x 600");
        displayScreen[i].x = (SCREEN_WIDTH / 3.0f);
        displayScreen[i].y = (SCREEN_HEIGHT / 3.0f) + 3 * (height * 1.10f);
        displayScreen[i].width = width;
        displayScreen[i].height = height;
        break;
      case 4:
        snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "640 x 480");
        displayScreen[i].x = (SCREEN_WIDTH / 3.0f);
        displayScreen[i].y = (SCREEN_HEIGHT / 3.0f) + 4 * (height * 1.10f);
        displayScreen[i].width = width;
        displayScreen[i].height = height;
        break;
      case 5:
        snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "Set Lightmode");
        displayScreen[i].x = (SCREEN_WIDTH / 3.0f) * 2.0f;
        displayScreen[i].y = (SCREEN_HEIGHT / 3.0f) + 2 * (height * 1.10f);
        displayScreen[i].width = width;
        displayScreen[i].height = height;
        break;
      case 6:
        snprintf(displayScreen[i].words, sizeof displayScreen[i].words, "Set Darkmode");
        displayScreen[i].x = (SCREEN_WIDTH / 3.0f) * 2.0f;
        displayScreen[i].y = (SCREEN_HEIGHT / 3.0f) + 3 * (height * 1.10f);
        displayScreen[i].width = width;
        displayScreen[i].height = height;
        break;
      }
    }
  }


  // use CP_Engine_SetNextGameState to specify this function as the initialization function
  // this function will be called once at the beginning of the program
  void OptionsInit(void)
  {
    InnerSelected = false;
    enteredFrame = true;
    setPause(false);
    initSliders();
    setupOptionsButtons();
    CP_Settings_RectMode(CP_POSITION_CENTER);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  }

  void drawSliders(void)
  {

    for (auto const&slider: audioSliders)
    {
      switch (getColorMode())
      {
      case DarkMode:
        CP_Settings_Fill(WHITE);
        break;
      case LightMode:
        CP_Settings_Fill(BLACK);
        break;
      }
      CP_Settings_TextSize(50 * SCREEN_HEIGHT / 1080.0f);
      CP_Font_DrawText(slider.title, slider.x, slider.y - 50);
      CP_Settings_Fill(GRAY_BUT);
      CP_Graphics_DrawRect(slider.x, slider.y, slider.length, 20);
      float x = (slider.length * slider.value);
      if (slider.selected == 1)
        CP_Settings_Fill(CP_Color_Create(50, 50, 50, 255));
      else
        CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));

      CP_Graphics_DrawRect(slider.x + x - (slider.length * .5f), slider.y, 20, 30);

      // Testing stuff
      // ------------------------------------------------
      // std::string buffer;
      // buffer = std::to_string(slider.value);
      // CP_Font_DrawText(buffer.c_str(), 300, slider.y);
      // ------------------------------------------------


    }
  }

  void checkSliders(void)
  {
    float mX = CP_Input_GetMouseX();
    float mY = CP_Input_GetMouseY();
    static int selected = 0;
    bool changed = checkControllerConectivity();
    for (int i = 0; i < AUDIO_SLIDERS; i++)
    {
      audioSliders[i].selected = false;
      if (!changed)
      {
        if (mX > audioSliders[i].x + (.5 * audioSliders[i].length) || mX < audioSliders[i].x - (.5 * audioSliders[i].length) || mY > audioSliders[i].y + (10) || mY < audioSliders[i].y - (10))
        {
          ;
        }
        else
        {
          selected = i;
          changed = true;
        }
      }
    }

    if (changed == false)
      selected = -1;

    if (isTriggered(YAxisPos))
    {
      --selected;
    }
    else if (isTriggered(YAxisNeg))
    {
      ++selected;
    }
    if (selected >= AUDIO_SLIDERS)
      selected = 0;
    if (changed == true && selected < 0)
      selected = AUDIO_SLIDERS - 1;
    float xRaw = 0;
    if (selected != -1)
      audioSliders[selected].selected = true;

    if (checkControllerConectivity())
    {
      float xRawS = getXRawDirectional(Left);
      float xRawK = getXRawDirectional(None);
      xRaw = (xRawS == 0) ? ((xRawK == 0) ? 0 : xRawK) : xRawS;
      xRaw /= 20;
    }
    else if (selected != -1)
    {
      float pos = getXRawDirectional(Mouse) - (audioSliders[selected].x - (SCREEN_WIDTH / 2.0f));
      if (abs(pos) < audioSliders[selected].x + audioSliders[selected].length / 2.0f && isPressed(Confirm))
      {
        xRaw = (pos / audioSliders[selected].length + .5f) - audioSliders[selected].value;
      }

    }

    if (selected == -1)
      return;
    audioSliders[selected].value += xRaw;

    if (audioSliders[selected].value > 1)
      audioSliders[selected].value = 1;
    if (audioSliders[selected].value < 0)
      audioSliders[selected].value = 0;

    switch (selected)
    {
    case 0:
      CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, audioSliders[selected].value);
      SetSounds(audioSliders[selected].value);
      break;
    case 1:
      CP_Sound_SetGroupVolume(CP_SOUND_GROUP_1, audioSliders[selected].value);
      SetMusic(audioSliders[selected].value);
      break;
    }




  }

  void drawOptionsButtons(void)
  {
    for (int i = 0; i < OPTIONSBUTTONS; i++)
    {
      if (optionsButton[i].selected == 1)
      {
        CP_Settings_Fill(GRAY);
        CP_Graphics_DrawRect(optionsButton[i].x, optionsButton[i].y, optionsButton[i].width, optionsButton[i].height);
        CP_Settings_Fill(WHITE);
        CP_Font_DrawText(optionsButton[i].words, optionsButton[i].x, optionsButton[i].y);
      }
      else
      {
        CP_Settings_Fill(BLACK);
        CP_Graphics_DrawRect(optionsButton[i].x, optionsButton[i].y, optionsButton[i].width, optionsButton[i].height);
        CP_Settings_Fill(WHITE);
        CP_Font_DrawText(optionsButton[i].words, optionsButton[i].x, optionsButton[i].y);
      }
    }
  }

  void checkOptionsButtons(void)
  {
    float mX = CP_Input_GetMouseX();
    float mY = CP_Input_GetMouseY();
    static int selected = 0;
    bool changed = checkControllerConectivity();
    screenSelections = &selected;
    for (int i = 0; i < OPTIONSBUTTONS; i++)
    {
      optionsButton[i].selected = false;
      if (!changed)
      {
        if (mX > optionsButton[i].x + (.5 * optionsButton[i].width) || mX < optionsButton[i].x - (.5 * optionsButton[i].width) || mY > optionsButton[i].y + (.5 * optionsButton[i].height) || mY < optionsButton[i].y - (.5 * optionsButton[i].height))
        {
          ;
        }
        else
        {
          changed = true;
          selected = i;
        }
      }
    }
    if (isTriggered(XAxisNeg))
    {
      --selected;
    }
    else if (isTriggered(XAxisPos))
    {
      ++selected;
    }
    if (changed == false)
      selected = -1;
    if (selected >= OPTIONSBUTTONS - 1)
      selected = 0;
    if (changed == true && selected < 0)
      selected = OPTIONSBUTTONS - 2;
    if (selected != -1)
    {
      optionsButton[selected].selected = true;
    }

    if (isPressed(Confirm))
    {
      switch (selected)
      {

      case 0:
        opsSubScreen = 0;
        break;
      case 1:
        opsSubScreen = 2;
        break;
      case 2:
        opsSubScreen = 3;

        break;
      case 3:
        if (getGame())
        {
          CP_Engine_SetNextGameStateForced(gameLoopInit, gameLoopUpdate, gameLoopExit);
          setOptions(false);
          setPause(true);
        }
        else
          CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
        break;
      default:
        break;
      }
      InnerSelected = true;
    }
  }

  void ckeckDisplayScreen()
  {
    float mX = CP_Input_GetMouseX();
    float mY = CP_Input_GetMouseY();
    static int selected = 0;
    bool changed = checkControllerConectivity();
  
    for (int i = 0; i < DISPLASY_BUTTONS; i++)
    {
      displayScreen[i].selected = false;
      if (!changed) 
      {
        if (mX > displayScreen[i].x + (.5 * displayScreen[i].width) || mX < displayScreen[i].x - (.5 * displayScreen[i].width) || mY > displayScreen[i].y + (.5 * displayScreen[i].height) || mY < displayScreen[i].y - (.5 * displayScreen[i].height))
        {
          ;
        }
        else
        {
          selected = i;
          changed = true;
        }
      }
    }

    if (isTriggered(YAxisPos))
    {
      --selected;
    }
    else if (isTriggered(YAxisNeg))
    {
      ++selected;
    }
    if (isTriggered(XAxisPos))
      selected = 5;
    else if (isTriggered(XAxisNeg))
      selected = 0;
    if (changed == false)
      selected = -1;
    if (selected >= DISPLASY_BUTTONS)
      selected = 0;
    if (changed == true && selected < 0)
      selected = DISPLASY_BUTTONS - 1;
    if (selected != -1)
    {
      displayScreen[selected].selected = true;
    }


    if (isPressed(Confirm))
    {
      switch (selected)
      {
      case 0:
        CP_System_FullscreenAdvanced(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
          SetFullScreen(true);
          break;
      case 1:
        CP_System_SetWindowSize(1920, 1080);
          SetWindowSize({ 1920, 1080 });
          SetFullScreen(false);
        break;
      case 2:
        CP_System_SetWindowSize(1080, 720);
        SetWindowSize({ 1080, 720 });
        SetFullScreen(false);
        break;
      case 3:
        CP_System_SetWindowSize(800, 600);
        SetWindowSize({ 800, 600 });
        SetFullScreen(false);
        break;
      case 4:
        CP_System_SetWindowSize(640, 480);
        SetWindowSize({ 640, 480 });
        SetFullScreen(false);
        break;
      case 5:
        setColorMode(LightMode);
        break;
      case 6:
        setColorMode(DarkMode);
        break;
      }
      setupOptionsButtons();
      initSliders();
    }

  }

  void drawDisplayScreen(void) 
  {
    char buffer[1000] = { 0 };
    switch (getColorMode()) 
    {
    case DarkMode:
      CP_Settings_Fill(WHITE);
      break;
    case LightMode:
      CP_Settings_Fill(BLACK);
      break;
    }

    snprintf(buffer, sizeof buffer, "The current window size is %i by %i", SCREEN_WIDTH, SCREEN_HEIGHT);
    CP_Font_DrawText(buffer, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * .20f);

    for (int i = 0; i < DISPLASY_BUTTONS; ++i) 
    {

      if (displayScreen[i].selected == 1)
      {
        CP_Settings_Fill(GRAY);
        CP_Graphics_DrawRect(displayScreen[i].x, displayScreen[i].y, displayScreen[i].width, displayScreen[i].height);
        CP_Settings_Fill(WHITE);
        CP_Font_DrawText(displayScreen[i].words, displayScreen[i].x, displayScreen[i].y);
      }
      else
      {
        CP_Settings_Fill(BLACK);
        CP_Graphics_DrawRect(displayScreen[i].x, displayScreen[i].y, displayScreen[i].width, displayScreen[i].height);
        CP_Settings_Fill(WHITE);
        CP_Font_DrawText(displayScreen[i].words, displayScreen[i].x, displayScreen[i].y);
      }
    }

  }


  void drawSubScreen(void)
  {

    char buffer[1000] = { 0 };
    switch (opsSubScreen)
    {
    case 0:
      drawDisplayScreen();
      if(InnerSelected)
        ckeckDisplayScreen();
      break;
    case 1:
      snprintf(buffer, sizeof buffer, "Wumpus is very lonely today");
      CP_Font_DrawText(buffer, SCREEN_WIDTH / 2.0f, 250.0f);
      break;
    case 2:;
    {
      float xpos = SCREEN_WIDTH / 4.0f;
      float ypos = SCREEN_HEIGHT / 3.0f;


      CP_Settings_Fill(CP_Color_Create(50, 50, 50, 255));
      /* Left Side */
      CP_Graphics_DrawRect(xpos, ypos, SCREEN_WIDTH / 35.0f, SCREEN_HEIGHT / 20.0f);
      CP_Graphics_DrawRect(xpos, ypos + 100 * SCREEN_WIDTH / 1920.0f, SCREEN_WIDTH / 35.0f, SCREEN_HEIGHT / 20.0f);
      CP_Graphics_DrawRect(xpos, ypos + 200 * SCREEN_WIDTH / 1920.0f, SCREEN_WIDTH / 35.0f, SCREEN_HEIGHT / 20.0f);
      CP_Graphics_DrawRect(xpos, ypos + 300 * SCREEN_WIDTH / 1920.0f, SCREEN_WIDTH / 35.0f, SCREEN_HEIGHT / 20.0f);
      CP_Graphics_DrawRect(xpos, ypos + 400 * SCREEN_WIDTH / 1920.0f, SCREEN_WIDTH / 15.0f, SCREEN_HEIGHT / 20.0f);

      /* Right Side */
      CP_Graphics_DrawRect(xpos * 3, ypos, SCREEN_WIDTH / 35.0f, SCREEN_HEIGHT / 20.0f);
      CP_Graphics_DrawRect(xpos * 3, ypos + 100 * SCREEN_WIDTH / 1920.0f, SCREEN_WIDTH / 35.0f, SCREEN_HEIGHT / 20.0f);
      CP_Graphics_DrawRect(xpos * 3, ypos + 200 * SCREEN_WIDTH / 1920.0f, SCREEN_WIDTH / 35.0f, SCREEN_HEIGHT / 20.0f);
      CP_Graphics_DrawRect(xpos * 3, ypos + 300 * SCREEN_WIDTH / 1920.0f, SCREEN_WIDTH / 10.0f, SCREEN_HEIGHT / 20.0f);



      CP_Graphics_DrawEllipse(xpos - 150 * SCREEN_WIDTH / 1920.0f, ypos + 400 * SCREEN_WIDTH / 1920.0f, 100 * SCREEN_WIDTH / 1920.0f, 150 * SCREEN_WIDTH / 1920.0f);
      CP_Graphics_DrawEllipse(xpos, ypos + 550 * SCREEN_WIDTH / 1920.0f, 100 * SCREEN_WIDTH / 1920.0f, 150 * SCREEN_WIDTH / 1920.0f);

      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Graphics_DrawEllipse(xpos - 150 * SCREEN_WIDTH / 1920.0f, ypos + 357 * SCREEN_WIDTH / 1920.0f, 20 * SCREEN_WIDTH / 1920.0f, 50 * SCREEN_WIDTH / 1920.0f);
      CP_Graphics_DrawEllipse(xpos, ypos + 507 * SCREEN_WIDTH / 1920.0f, 20 * SCREEN_WIDTH / 1920.0f, 50 * SCREEN_WIDTH / 1920.0f);

      CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
      CP_Graphics_DrawEllipse(xpos - 175 * SCREEN_WIDTH / 1920.0f, ypos + 357 * SCREEN_WIDTH / 1920.0f, 30 * SCREEN_WIDTH / 1920.0f, 30 * SCREEN_WIDTH / 1920.0f);

      CP_Font_DrawText("W", xpos, ypos);
      CP_Font_DrawText("A", xpos,     ypos + 100 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("S", xpos,     ypos + 200 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("D", xpos,     ypos + 300 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("R", xpos * 3, ypos);
      CP_Font_DrawText("Q", xpos * 3, ypos + 100 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("E", xpos * 3, ypos + 200 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Space", xpos, ypos + 400 * SCREEN_WIDTH / 1920.0f);


      switch (getColorMode())
      {
      case DarkMode:
        CP_Settings_Fill(WHITE);
        break;
      case LightMode:
        CP_Settings_Fill(BLACK);
        break;
      }
      CP_Font_DrawText("Left Mouse",       xpos     - 150 * SCREEN_WIDTH / 1920.0f, ypos + 400 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Move up",          xpos     + 300 * SCREEN_WIDTH / 1920.0f, ypos);
      CP_Font_DrawText("Move left",        xpos     + 300 * SCREEN_WIDTH / 1920.0f, ypos + 100 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Move down",        xpos     + 300 * SCREEN_WIDTH / 1920.0f, ypos + 200 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Move right",       xpos     + 300 * SCREEN_WIDTH / 1920.0f, ypos + 300 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Shoot",            xpos     + 300 * SCREEN_WIDTH / 1920.0f, ypos + 400 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Aim (Move mouse)", xpos     + 300 * SCREEN_WIDTH / 1920.0f, ypos + 550 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Reload",           xpos * 3 + 300 * SCREEN_WIDTH / 1920.0f, ypos);
      CP_Font_DrawText("Switch Weapons",   xpos * 3 + 300 * SCREEN_WIDTH / 1920.0f, ypos + 100 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Open Inventory",   xpos * 3 + 300 * SCREEN_WIDTH / 1920.0f, ypos + 200 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Dash",             xpos * 3 + 300 * SCREEN_WIDTH / 1920.0f, ypos + 300 * SCREEN_WIDTH / 1920.0f);
      CP_Font_DrawText("Shift",            xpos * 3,                                ypos + 300 * SCREEN_WIDTH / 1920.0f);
      CP_Settings_TextSize(20 * SCREEN_WIDTH / 1920.0f);
      drawWords("Rebinding to be added at a later date", xpos * 3, ypos * 2.25f, 50 * SCREEN_WIDTH / 1920.0f, (getColorMode() == LightMode) ? BLACK : WHITE);
    }
      break;
    case 3:

      drawSliders();
      if(InnerSelected)
        checkSliders();

      break;
    }
  }

  // use CP_Engine_SetNextGameState to specify this function as the update function
  // this function will be called repeatedly every frame
  void OptionsUpdate(void)
  {
    if(enteredFrame)
    {
      enteredFrame = false;
      return;
    }
    CP_Settings_TextSize(40 * (SCREEN_WIDTH / 1920.0f));
    switch (getColorMode())
    {
    case DarkMode:
      CP_Graphics_ClearBackground(CP_Color_CreateHex(0x7494bfff));
      break;
    case LightMode:
      CP_Graphics_ClearBackground(CP_Color_CreateHex(0xf2e7efff));
      break;
    }
      drawOptionsButtons();
    if (!InnerSelected || !checkControllerConectivity())
    {
      checkOptionsButtons();
    }
    drawSubScreen();
    if (isPressed(Back))
      InnerSelected = false;
    if (isPressed(Confirm))
      InnerSelected = true;
    if (!InnerSelected) 
    {
      for (auto& slider : audioSliders)
        slider.selected = false;
      for (auto& button : displayScreen)
        button.selected = false;
    }
    else if (InnerSelected)
      for (auto& button : optionsButton)
        button.selected = false;
  }

  // use CP_Engine_SetNextGameState to specify this function as the exit function
  // this function will be called once just before leaving the current gamestate
  void OptionsExit(void)
  {
    *screenSelections = 0;
  }
}