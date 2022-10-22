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
#define MAX_ANIME_ENEMIES 5
#define MAX_ANIME_BULLETS 200
#define BULLET_SPEED 10
void initAnim(void);

button buttons[BUTTONS] = { 0 };
enemy animations[MAX_ANIME_BULLETS] = { 0 };
enemy Player = { 0 };
bullet animBullets[MAX_ANIME_BULLETS] = { 0 };

void setupButtons()
{
  CP_Settings_TextSize(50);
  for (int i = 0; i < BUTTONS; i++)
  {
    switch (i)
    {
    case 0:
      snprintf(buttons[i].words, sizeof buttons[i].words, "START!");
      buttons[i].x = (CP_System_GetWindowWidth() / 2.0f);
      buttons[i].y = (CP_System_GetWindowHeight() / 2.5f);
      buttons[i].width = 300;
      buttons[i].height = 100;
      break;
    case 1:
      snprintf(buttons[i].words, sizeof buttons[i].words, "OPTIONS!");
      buttons[i].x = (CP_System_GetWindowWidth() / 2.0f);
      buttons[i].y = (CP_System_GetWindowHeight() /2.0f);
      buttons[i].width = 300;
      buttons[i].height = 100;
      break;
    case 2:
      snprintf(buttons[i].words, sizeof buttons[i].words, "EXIT!");
      buttons[i].x = (CP_System_GetWindowWidth() / 2.0f);
      buttons[i].y = (CP_System_GetWindowHeight() / 1.4f);
      buttons[i].width = 300;
      buttons[i].height = 100;
      break;
    case 3:
      snprintf(buttons[i].words, sizeof buttons[i].words, "CREDITS!");
      buttons[i].x = (CP_System_GetWindowWidth() / 2.0f);
      buttons[i].y = (CP_System_GetWindowHeight() / 1.65f);
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
  initAnim();
  setupButtons();
  CP_Settings_RectMode(CP_POSITION_CENTER);
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
}

void drawAnimBullets(void)
{
  for (int i = 0; i < MAX_ANIME_BULLETS; i++)
  {
    if (animBullets[i].active == 0)
      continue;
    if (animBullets[i].users == 0)
      CP_Settings_Fill(CP_Color_Create(10, 10, 25, 255));
    else if (animBullets[i].users == 1)
      CP_Settings_Fill(CP_Color_Create(201, 254, 255, 255));

    CP_Settings_StrokeWeight(0.0f);
    CP_Graphics_DrawCircle(animBullets[i].x + (CP_System_GetWindowWidth() / 2.0f), -animBullets[i].y + (CP_System_GetWindowHeight() / 2.0f), animBullets[i].radius);
  }
}

int animShoot(enemy a)
{
  float dir[2] = { 0,0 };
  dir[0] = Player.x - a.x;
  dir[1] = Player.y - a.y;
  float magnitude = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
  dir[0] /= magnitude;
  dir[1] /= magnitude;
  for (int i = 0; i < MAX_ANIME_BULLETS; i++)
  {
    if (animBullets[i].active == 0)
    {
      animBullets[i].radius = 10;
      animBullets[i].active = 1;
      animBullets[i].x = a.x;
      animBullets[i].y = a.y;
      animBullets[i].dir[0] = dir[0];
      animBullets[i].dir[1] = dir[1];
      animBullets[i].life = 10;
      animBullets[i].pwr = CP_Random_RangeInt(5, 25);
      animBullets[i].users = 0;
      animBullets[i].speed = BULLET_SPEED;
      break;
    }
  }
  return 0;
}

int animeShooty(void)
{
  for (int i = 0; i < MAX_ANIME_ENEMIES; i++)
  {
    if (animations[i].active == 0)
      continue;
    if (animations[i].cooldown <= 0)
    {
      animShoot(animations[i]);
      switch (animations[i].type)
      {
      case 0:
        animations[i].cooldown = 5.0f;
        break;
      case 1:
        animations[i].cooldown = .5f;
        break;
      }
    }
    float d = CP_System_GetDt();
    if (d > 1)
      animations[i].cooldown -= .03f;
    else
      animations[i].cooldown -= d;
  }
  return 0;
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

void moveAnim(void) 
{
  for (int i = 0; i < MAX_ANIME_ENEMIES; i++)
  {
    if (animations[i].active == 0)
      continue;
    if (animations[i].dir[0] <= 10 && animations[i].dir[1] <= 10)
    {
      animations[i].dir[0] = CP_Random_RangeFloat(-100, 100);
      if (animations[i].x + animations[i].dir[0] > CP_System_GetWindowWidth() / 2.0 || animations[i].x + animations[i].dir[0] < -CP_System_GetWindowWidth() / 2.0f)
        animations[i].dir[0] = -animations[i].dir[0];
      animations[i].dir[1] = CP_Random_RangeFloat(-100, 100);
      if (animations[i].y + animations[i].dir[1] > CP_System_GetWindowHeight() / 2.0 || animations[i].y + animations[i].dir[1] < -CP_System_GetWindowHeight() / 2.0f)
        animations[i].dir[1] = -animations[i].dir[1];
    }
    animations[i].x += animations[i].dir[0] * CP_System_GetDt();
    animations[i].y += animations[i].dir[1] * CP_System_GetDt();
    animations[i].dir[0] += -animations[i].dir[0] * CP_System_GetDt();
    animations[i].dir[1] += -animations[i].dir[1] * CP_System_GetDt();
  }

  if (Player.dir[0] <= 20 && Player.dir[1] <= 20)
  {

    Player.dir[0] = CP_Random_RangeFloat(-100, 100);
    if (Player.x + Player.dir[0] > CP_System_GetWindowWidth() / 2.0 || Player.x + Player.dir[0] < -CP_System_GetWindowWidth() / 2.0f)
      Player.dir[0] = -Player.dir[0];
    Player.dir[1] = CP_Random_RangeFloat(-100, 100);
    if (Player.y + Player.dir[1] > CP_System_GetWindowHeight() / 2.0 || Player.y + Player.dir[1] < -CP_System_GetWindowHeight() / 2.0f)
      Player.dir[1] = -Player.dir[1];
  }
  Player.x += Player.dir[0] * CP_System_GetDt();
  Player.y += Player.dir[1] * CP_System_GetDt();
  Player.dir[0] += -Player.dir[0] * CP_System_GetDt();
  Player.dir[1] += -Player.dir[1] * CP_System_GetDt();
}

void drawAnimEnemies(void)
{
  CP_Settings_Fill(CP_Color_Create(226, 225, 76, 255));
  CP_Settings_StrokeWeight(1.5f);
  for (int i = 0; i < MAX_ANIME_ENEMIES; i++)
  {
    if (animations[i].active == 0)
      continue;
    CP_Graphics_DrawCircle(animations[i].x + (CP_System_GetWindowWidth() / 2.0f), -animations[i].y + (CP_System_GetWindowHeight() / 2.0f), animations[i].radius);
  }
  CP_Settings_StrokeWeight(1.5f);
  CP_Settings_Fill(CP_Color_CreateHex(0x99A3A4));
  CP_Graphics_DrawCircle(Player.x + (CP_System_GetWindowWidth() / 2.0f), -Player.y + (CP_System_GetWindowHeight() / 2.0f), Player.radius);

}

void initAnim(void) 
{
  for (int i = 0; i < MAX_ANIME_ENEMIES; i++)
  {

    float Ex = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
    float Ey = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
    animations[i] = (enemy){ 1, Ex, Ey };
    animations[i].speed = 50;
    animations[i].cooldown = 1.5f;
    animations[i].type = CP_Random_RangeInt(0, 1);
    animations[i].radius = 25;
    animations[i].health = 60;
  }
  float Ex = CP_Random_RangeFloat(-CP_System_GetWindowWidth() / 2.0f + 200, CP_System_GetWindowWidth() / 2.0f - 200);
  float Ey = CP_Random_RangeFloat(-CP_System_GetWindowHeight() / 2.0f + 200, CP_System_GetWindowHeight() / 2.0f - 200);
  Player = (enemy){ 1, Ex, Ey };  
  Player.speed = 50;
  Player.cooldown = 1.5f;
  Player.type = CP_Random_RangeInt(0, 1);
  Player.radius = 25;
  Player.health = 60;
}
int moveAnimBullets(void)
{
  for (int i = 0; i < MAX_ANIME_BULLETS; i++)
  {
    if (animBullets[i].active)
    {
      animBullets[i].x += (animBullets[i].dir[0] * animBullets[i].speed);
      animBullets[i].y += (animBullets[i].dir[1] * animBullets[i].speed);
      animBullets[i].life -= CP_System_GetDt();
      if (animBullets[i].life <= 0)
      {
        animBullets[i].active = 0;
        animBullets[i].users = 0;
        animBullets[i].speed = BULLET_SPEED;
      }
    }
  }
  return 0;
}

void menuAnimation(void) 
{
  moveAnimBullets();
  drawAnimBullets();
  drawAnimEnemies();
  moveAnim();
  animeShooty();

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
  CP_Font_DrawText("LIVE TO SURVIVE", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 5.0f);
  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
  CP_Settings_TextSize(200);
  CP_Font_DrawText("LIVE TO SURVIVE", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 5.0f);
  CP_Settings_TextSize(50);
  menuAnimation();
  drawButtons();

  checkButtons();
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void MainMenuExit(void)
{

}
