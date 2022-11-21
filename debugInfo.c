#include "cprocessing.h"
#include "structs.h"
#include "gameLoop.h"
#include <stdio.h>


void drawDebugInfo(player *pl, enemy *en) 
{
  char buffer[200];
  CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
  CP_Settings_TextSize(15);
  snprintf(buffer, sizeof buffer, "%.3f", CP_System_GetFrameRate());
  CP_Font_DrawText(buffer, CP_System_GetWindowWidth() - 50.0f, 20.0f);
  snprintf(buffer, sizeof buffer, "%04.3f, %04.3f", pl->x, pl->y);
  CP_Font_DrawText(buffer, CP_System_GetWindowWidth() - 100.0f, 40.0f);
  CP_Font_DrawText("Enemies", CP_System_GetWindowWidth() - 100.0f, 50.0f);
  for (int i = 0; i < MAX_ENEMIES; i++) 
  {
    if (en[i].active == 0)
      continue;
    snprintf(buffer, sizeof buffer, "%04.3f, %04.3f", en[i].dir[0], en[i].dir[1]);
    CP_Font_DrawText(buffer, CP_System_GetWindowWidth() - 100.0f, 60.0f+ (15 * i));
  }
  CP_Settings_Stroke(CP_Color_CreateHex(0xffed00ff));
  CP_Settings_StrokeWeight(5);
  //CP_Graphics_DrawLine(CP_System_GetWindowWidth()/2.0f,CP_System_GetWindowHeight() / 2.0f, pl->direction[0] + CP_System_GetWindowWidth() / 2.0f, (pl->direction[1]) + CP_System_GetWindowHeight() / 2.0f);
  CP_Settings_Stroke(CP_Color_CreateHex(0x000000ff));
}

void drawDebugLine(int x1, int x2, int y1, int y2) 
{
  camera *c = retCam();
  CP_Settings_Stroke(CP_Color_CreateHex(0xffed00ff));
  CP_Settings_StrokeWeight(5);
  CP_Graphics_DrawLine((x1 - c->x) + CP_System_GetWindowWidth() / 2.0f, -(y1 - c->y) + CP_System_GetWindowHeight() / 2.0f, (x2 - c->x) + CP_System_GetWindowWidth() / 2.0f, -(y2 - c->y) + CP_System_GetWindowHeight() / 2.0f);
  CP_Settings_Stroke(CP_Color_CreateHex(0x000000ff));
}