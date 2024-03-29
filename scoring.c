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
// Copyright � 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "structs.h"
#include "mainMenu.h"
#include "globalData.h"
#include "checkStuff.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ShlObj_core.h>
#include <fileapi.h>
#include <shtypes.h>

#define MAX_CHARS 100
#define MAX_LINES 100

static char path[100] = { 0 };
static int pos = 0;
static int userWritten = 0;
static button submitButton;
static FILE* leaderboard;
static char buffer[MAX_CHARS] = { 0 };;
static char names[MAX_LINES][MAX_CHARS] = { 0 };
static char times[MAX_LINES][MAX_CHARS] = { 0 };;
static char userInput[MAX_CHARS] = { 0 };;
void readLeaderBoard(void)
{

  char* name = NULL;
  char* time = NULL;
  char* nextSet = NULL;
  const char sep[] = { ":" };
  int i = 0;
  do
  {
    fgets(buffer, 100, leaderboard);
    name = strtok_s(buffer, sep, &nextSet);
    strcat_s(names[i], sizeof(names[i]), name);
    size_t x = strlen(names[i]);
    names[i][x + 1] = '\0';
    time = strtok_s(NULL, sep, &nextSet);
    strcat_s(times[i], sizeof(times[i]), time);
    x = strlen(times[i]);
    times[i][x + 1] = '\0';
   
    i++;
  } while ((names[i][0] = (char)fgetc(leaderboard)) != EOF && i < MAX_LINES);
  names[i][0] = '\0';
  fclose(leaderboard);
}

void initsButtons(void) 
{
	submitButton.height = 100;
	submitButton.width = 300;
	submitButton.x = SCREEN_WIDTH / 2.0f;
	submitButton.y = SCREEN_HEIGHT / 1.3f;
	snprintf(submitButton.words, sizeof submitButton.words, "Submit");
}

void initInputStrings(void) 
{
  snprintf(userInput, sizeof userInput, "");
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void scoresInit(void)
{
  initInputStrings();
  initsButtons();
  wchar_t* loc = 0;
  SHGetKnownFolderPath(&FOLDERID_Documents, 0, NULL, &loc);
  size_t a;
  wcstombs_s(&a, path, _countof(path), loc, _countof(path) - 1);
  CoTaskMemFree((void*)loc);
  strcat_s(path, _countof(path), "\\DescentGames");
  CreateDirectory(path, NULL);
  strcat_s(path, _countof(path), "\\Simply_Survive");
  CreateDirectory(path, NULL);
  strcat_s(path, _countof(path), "\\Leaderboard.txt");

  fopen_s(&leaderboard, path, "r");
  if (leaderboard == NULL)
    return;
  readLeaderBoard();
  //fopen_s(&leaderboardWrite, "./Leaderboard", "ax");
}

void drawLeaderBoard(void) 
{
  CP_Settings_TextSize(20);
  for (int i = 0; i < MAX_LINES; i++)
  {
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText(names[i], SCREEN_WIDTH/3.0f, 100 + (30.0f * i));
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_MIDDLE);
    CP_Font_DrawText(times[i], SCREEN_WIDTH / 1.5f, 100 + (30.0f * i));
  }
}


void drawButtonss(void)
{
  if (submitButton.selected == 1)
  {
    CP_Settings_Fill(CP_Color_Create(255, 100, 255, 255));
    CP_Graphics_DrawRect(submitButton.x, submitButton.y, submitButton.width, submitButton.height);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(submitButton.words, submitButton.x, submitButton.y);
  }
  else
  {
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRect(submitButton.x, submitButton.y, submitButton.width, submitButton.height);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(submitButton.words, submitButton.x, submitButton.y);
  }
}
void readInput(void) 
{

  checkAlphanumericKeys(userInput, &pos, MAX_CHARS);
}

int saveScore(void) 
{
  if (fopen_s(&leaderboard, path, "w") != 0)
    return 100;
  char buf[100] = { 0 };
  for (int i = 0; i < MAX_LINES; i++) 
  {
    if (names[i][0] == (char)0)
      continue;

    float f;

    f = (float)atof(times[i]);
    if (f < getTime() && userWritten == 0)
    {

      snprintf(buf, sizeof buf, "%s : %f\n", userInput, getTime());
      fprintf(leaderboard, buf);
      snprintf(buf, sizeof buf, "%s : %s", names[i], times[i]);
      fprintf(leaderboard, buf);
      userWritten = 1;
    }
    else 
    {
      snprintf(buf, sizeof buf, "%s : %s", names[i], times[i]);
      fprintf(leaderboard, buf);
    }

  }
  if (userWritten == 0)
  {
    snprintf(buf, sizeof buf, "%s : %f", userInput, getTime());
    fprintf(leaderboard, buf);
  }
  fclose(leaderboard);
  leaderboard = NULL;
  return 0;
}

void checkButtonss(void)
{
  float mX = CP_Input_GetMouseX();
  float mY = CP_Input_GetMouseY();

  if (mX > submitButton.x + (.5 * submitButton.width) || mX < submitButton.x - (.5 * submitButton.width) || mY > submitButton.y + (.5 * submitButton.height) || mY < submitButton.y - (.5 * submitButton.height))
  {
    submitButton.selected = 0;
  }
  else
  {
    submitButton.selected = 1;
  }
  if (submitButton.selected)
  {
    if (CP_Input_MouseClicked())
    {
      saveScore();
      CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
    }
  }

}
// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void scoresUpdate(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));
  drawButtonss();
  drawLeaderBoard();
	CP_Settings_TextSize(40);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  readInput();
  char buff[100];
  snprintf(buff, sizeof buff, "Your Time: %.3f seconds", getTime());
  CP_Font_DrawText(buff, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.1f);
  snprintf(buff, sizeof buff, "Name: %s", userInput);
  CP_Font_DrawText(buff, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.2f);
  checkButtonss();

}



// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void scoresExit(void)
{

}
