#include "drawStuff.h"
#include "structs.h"
#include "cprocessing.h"
#include "stdio.h"
#include <math.h>

void drawPlayer(player pl, camera c)
{
  CP_Settings_StrokeWeight(1.5f);
  CP_Settings_Fill(CP_Color_CreateHex(0x22A3A4FF));
  CP_Graphics_DrawCircle((pl.x - c.x) + (CP_System_GetWindowWidth() / 2.0f), -(pl.y - c.y) + (CP_System_GetWindowHeight() / 2.0f), pl.playerRadius * 2);
  CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
  CP_Graphics_DrawRectAdvanced(((pl.x - c.x) + ((pl.direction[0] * pl.playerRadius) / sqrtf(pl.direction[0] * pl.direction[0] + pl.direction[1] * pl.direction[1]))) + (CP_System_GetWindowWidth() / 2.0f), (-(pl.y - c.y) + ((pl.direction[1] * pl.playerRadius) / sqrtf(pl.direction[0] * pl.direction[0] + pl.direction[1] * pl.direction[1]))) + (CP_System_GetWindowHeight() / 2.0f), pl.playerRadius / 1.25f, 10, pl.rot, 0.0f);
}


void drawWeapon(int weapon, float powerUpTimer, int powerUp)
{
  float screen_center = CP_System_GetWindowWidth() / 2.0f;
  CP_Settings_Fill(CP_Color_Create(220, 220, 220, 100));
  CP_Graphics_DrawRectAdvanced(screen_center, 50, 500, 200, 0, 45);
  switch (weapon)
  {
  case 0:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center, 30, 100, 20, 0, 10);

    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center + 30, 55, 30, 55, -15, 10);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("Pistol", screen_center, 120);
    break;
  case 4:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center, 30, 100, 20, 0, 10);
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center + 30, 55, 30, 55, -15, 10);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("Pistol (Semi-Auto)", screen_center, 120);
    break;
  case 1:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center - 20, 40, 140, 20, 0, 10);
    CP_Graphics_DrawRectAdvanced(screen_center + 60, 50, 70, 20, 0, 10);
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center + 30, 60, 25, 50, -15, 10);
    CP_Graphics_DrawRectAdvanced(screen_center - 10, 60, 25, 30, -10, 5);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("Auto Rifle", screen_center, 120);
    break;
  case 2:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center - 30, 40, 250, 20, 0, 10);
    CP_Graphics_DrawRectAdvanced(screen_center + 70, 50, 90, 20, 0, 10);
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center - 25, 55, 60, 20, 0, 5);
    CP_Graphics_DrawRectAdvanced(screen_center, 25, 60, 15, 0, 5);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("1.5\' Long boii Hours", screen_center, 120);
    break;
  case 3:
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
    CP_Graphics_DrawRectAdvanced(screen_center - 20, 40, 140, 20, 0, 10);
    CP_Graphics_DrawRectAdvanced(screen_center + 60, 50, 70, 20, 0, 10);
    CP_Settings_Fill(CP_Color_Create(60, 60, 60, 255));
    CP_Graphics_DrawRectAdvanced(screen_center + 30, 60, 25, 30, -25, 10);
    CP_Graphics_DrawRectAdvanced(screen_center - 25, 55, 60, 20, 0, 5);
    CP_Settings_TextSize(40);
    CP_Font_DrawText("Equiped", screen_center, 90);
    CP_Font_DrawText("Shoty 5k", screen_center, 120);
    break;
  }
  char buffer[100];
  if (powerUpTimer > 0)
  {
    switch (powerUp)
    {
    case 1:
      snprintf(buffer, sizeof buffer, "Instal Kill: %f", powerUpTimer);
      break;
    case 2:
      snprintf(buffer, sizeof buffer, "Rapid Fire: %f", powerUpTimer);
      break;
    case 5:
      snprintf(buffer, sizeof buffer, "Invincibility: %f", powerUpTimer);
      break;
    }
    CP_Font_DrawText(buffer, screen_center, 300);
  }
}

void drawBullets(bullet* bullets, camera C)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active == 0)
      continue;
    if (bullets[i].users == 0)
      CP_Settings_Fill(CP_Color_Create(10, 10, 25, 255));
    else if (bullets[i].users == 1)
      CP_Settings_Fill(CP_Color_Create(201, 254, 255, 255));

    CP_Settings_StrokeWeight(0.0f);
    CP_Graphics_DrawCircle((bullets[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(bullets[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f), bullets[i].radius);
  }
}

void drawEnemies(enemy* en, camera C)
{
  CP_Settings_Fill(CP_Color_Create(226, 225, 76, 255));
  for (int i = 0; i < MAX_ENEMIES; i++)
  {
    if (en[i].active == 0)
      continue;
    CP_Graphics_DrawCircle((en[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(en[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f), en[i].radius);
  }
}

void drawBosses(enemy* bosses, camera C)
{
  CP_Settings_Fill(CP_Color_Create(50, 100, 230, 255));
  for (int i = 0; i < MAX_BOSSESS; i++)
  {
    if (bosses[i].active == 0)
      continue;
    CP_Graphics_DrawCircle((bosses[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(bosses[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f), bosses[i].radius);
  }
}

void drawPickupText(string* pickupText, camera C)
{
  CP_Settings_TextSize(30);
  for (int i = 0; i < MAX_TEXT; i++)
  {
    if (pickupText[i].active == 0)
      continue;
    if (pickupText[i].life <= 0)
    {
      pickupText[i].active = 0;
      continue;
    }
    CP_Settings_Fill(CP_Color_Create(100, 100, 100, 125));
    CP_Font_DrawText(pickupText[i].buffer, (pickupText[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(pickupText[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f));
    pickupText[i].y++;
    pickupText[i].life -= CP_System_GetDt();
  }
}

void drawItems(item* items, camera C)
{
  CP_Settings_StrokeWeight(0.0f);
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
      continue;

    switch (items[i].type)
    {
    case 0:
      CP_Settings_Fill(CP_Color_Create(19, 136, 8, 255));
      CP_Graphics_DrawRectAdvanced((items[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(items[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f), items[i].radius, items[i].radius, 45.0f, 0.0f);
      CP_Settings_TextSize(10);
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Font_DrawText("Health", (items[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(items[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f));
      break;
    case 1:
      CP_Settings_Fill(CP_Color_Create(201, 242, 201, 255));
      CP_Graphics_DrawRectAdvanced((items[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(items[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f), items[i].radius, items[i].radius, 0, 5);
      CP_Settings_TextSize(10);
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Font_DrawText("Weapon", (items[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(items[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f));
      break;
    case 2:
      CP_Settings_Fill(CP_Color_Create(250, 100, 255, 255));
      CP_Graphics_DrawRectAdvanced((items[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(items[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f), items[i].radius, items[i].radius, 0, 5);
      CP_Settings_TextSize(10);
      CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
      CP_Font_DrawText("POWERUP", (items[i].x - C.x) + (CP_System_GetWindowWidth() / 2.0f), -(items[i].y - C.y) + (CP_System_GetWindowHeight() / 2.0f));
      break;
    }
  }
}

void drawBuildings(building *buildings, camera c) 
{
  CP_Settings_Fill(CP_Color_Create(10, 10, 10, 255));
  for (int i = 0; i < NUMBER_OF_BUILDINGS; i++) 
  {
    if (fabsf(buildings[i].x - c.x) > fabsf(CP_System_GetWindowWidth() / 2.0f) + 300 || fabsf(buildings[i].y - c.y) > fabsf(CP_System_GetWindowHeight() / 2.0f) + 300)
      continue;

    CP_Graphics_DrawRect((buildings[i].x - c.x) + (CP_System_GetWindowWidth() / 2.0f), -(buildings[i].y - c.y) + (CP_System_GetWindowHeight() / 2.0f),  buildings[i].xLen, buildings[i].yLen);
  }
}