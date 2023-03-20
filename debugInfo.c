#include "cprocessing.h"
#include "structs.h"
#include "gameLoop.h"
#include "globalData.h"

#include <stdio.h>
#include <stdlib.h>


static char* buffer = NULL;

void drawDebugInfo(player *pl, enemy *en) 
{
  if (buffer == NULL)
    buffer = malloc(200 * sizeof(char));
  
  if (buffer == NULL)
    return;
  int i = 0;
  CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
  CP_Settings_TextSize(15);
  snprintf(buffer, 200, "%.3f", CP_System_GetFrameRate());
  CP_Font_DrawText(buffer, SCREEN_WIDTH - 50.0f, 20.0f);
  snprintf(buffer, 200, "%04.3f, %04.3f", pl->x, pl->y);
  CP_Font_DrawText(buffer, SCREEN_WIDTH - 100.0f, 40.0f);
  CP_Font_DrawText("Enemies", SCREEN_WIDTH - 100.0f, 50.0f);
  while(en)
  {
    snprintf(buffer, 200, "%04.3f, %04.3f", en->dir.x, en->dir.y);
    CP_Font_DrawText(buffer, SCREEN_WIDTH - 100.0f, 60.0f+ (15 * i));
    i++;
    en = en->next;
  }
  CP_Settings_Stroke(CP_Color_CreateHex(0xffed00ff));
  CP_Settings_StrokeWeight(5);
  //CP_Graphics_DrawLine(SCREEN_WIDTH/2.0f,SCREEN_HEIGHT / 2.0f, pl->direction[0] + SCREEN_WIDTH / 2.0f, (pl->direction[1]) + SCREEN_HEIGHT / 2.0f);
  CP_Settings_Stroke(BLACK);

  snprintf(buffer, sizeof buffer, "%i", getBulletCount());
  CP_Font_DrawText(buffer, SCREEN_WIDTH - 50.0f, 200.0f);
}

void drawDebugLine(int x1, int x2, int y1, int y2) 
{
  camera *c = retCam();
  CP_Settings_Stroke(CP_Color_CreateHex(0xffed00ff));
  CP_Settings_StrokeWeight(5);
  CP_Graphics_DrawLine((x1 - c->x) + SCREEN_WIDTH / 2.0f, -(y1 - c->y) + SCREEN_HEIGHT / 2.0f, (x2 - c->x) + SCREEN_WIDTH / 2.0f, -(y2 - c->y) + SCREEN_HEIGHT / 2.0f);
  CP_Settings_Stroke(CP_Color_CreateHex(0x000000ff));
}