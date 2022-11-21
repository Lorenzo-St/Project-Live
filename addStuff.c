#include "structs.h"
#include "cprocessing.h"
#include "checkStuff.h"
#include <stdio.h>


int addPickup(item a, string* pickupText)
{
  for (int i = 0; i < MAX_TEXT; i++)
  {
    if (pickupText[i].active)
      continue;

    pickupText[i].active = 1;
    pickupText[i].life = 1.5f;
    pickupText[i].x = a.x;
    pickupText[i].y = a.y;
    switch (a.type)
    {
    case 0:
      snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Health Aquired");
      break;
    case 1:
      switch (a.containes)
      {
      case 1:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Picked up Autorifle");
        break;
      case 2:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Picked up Sniper");
        break;
      case 3:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Picked up Shotgun");
        break;
      case 4:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "Picked up Semi-auto Pistol");
        break;
      }
      break;
    case 2:
      switch (a.containes)
      {
      case 1:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "INSTA-KILL!!");
        break;
      case 2:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "RAPID FIRE!!");
        break;
      case 3:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "FULL HEAL!!");
        break;
      case 4:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "KA-BOOOM!!!!");
        break;
      case 5:
        snprintf(pickupText[i].buffer, sizeof pickupText[i].buffer, "INVINCIBILITY!!");
        break;
      }
      break;
    }
  }
  return 0;
}

void addEnemy(int bossesEnabled, enemy* en, enemy* bosses, building *buildings, camera c)
{
  if (bossesEnabled == 0)
  {
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
      if (en[i].active == 1)
        continue;
      if (en[i].active == 0)
      {
        en[i].active = 1;
        en[i].x = CP_Random_RangeFloat(c.x - CP_System_GetWindowWidth() / 2.0f , c.x + CP_System_GetWindowWidth() / 2.0f);
        en[i].y = CP_Random_RangeFloat(c.y  - CP_System_GetWindowHeight() / 2.0f, c.y +  CP_System_GetWindowHeight() / 2.0f);
        while (checkInsideBuilding(buildings, 1, en[i]) != 0) 
        {
          en[i].x = CP_Random_RangeFloat(c.x - CP_System_GetWindowWidth() / 2.0f, c.x + CP_System_GetWindowWidth() / 2.0f);
          en[i].y = CP_Random_RangeFloat(c.y - CP_System_GetWindowHeight() / 2.0f, c.y + CP_System_GetWindowHeight() / 2.0f);
        }
        en[i].speed = 50;
        en[i].cooldown = 1.5f;
        en[i].type = CP_Random_RangeInt(0, 1);
        en[i].health = 60;
        break;
      }
    }
  }
  else
  {
    int side = CP_Random_RangeInt(0, 100);
    if (side > 25)
    {
      for (int i = 0; i < MAX_ENEMIES; i++)
      {
        if (en[i].active == 1)
          continue;
        if (en[i].active == 0)
        {
          en[i].active = 1;
          en[i].x = CP_Random_RangeFloat(c.x - CP_System_GetWindowWidth() / 2.0f, c.x + CP_System_GetWindowWidth() / 2.0f);
          en[i].y = CP_Random_RangeFloat(c.y - CP_System_GetWindowHeight() / 2.0f, c.y + CP_System_GetWindowHeight() / 2.0f);
          while (checkInsideBuilding(buildings, 1, en[i]) != 0)
          {
            en[i].x = CP_Random_RangeFloat(c.x - CP_System_GetWindowWidth() / 2.0f, c.x + CP_System_GetWindowWidth() / 2.0f);
            en[i].y = CP_Random_RangeFloat(c.y - CP_System_GetWindowHeight() / 2.0f, c.y + CP_System_GetWindowHeight() / 2.0f);
          }
          en[i].speed = 50;
          en[i].cooldown = 1.5f;
          en[i].type = CP_Random_RangeInt(0, 1);
          en[i].health = 60;
          break;
        }
      }

    }
    else if (side <= 25)
    {
      for (int i = 0; i < MAX_BOSSESS; i++)
      {
        if (bosses[i].active == 1)
          continue;
        if (bosses[i].active == 0)
        {
          bosses[i].active = 1;
          bosses[i].x = CP_Random_RangeFloat(c.x - CP_System_GetWindowWidth() / 2.0f, c.x + CP_System_GetWindowWidth() / 2.0f);
          bosses[i].y = CP_Random_RangeFloat(c.y - CP_System_GetWindowHeight() / 2.0f, c.y + CP_System_GetWindowHeight() / 2.0f);
          while (checkInsideBuilding(buildings, 1, bosses[i]) != 0)
          {
            bosses[i].x = CP_Random_RangeFloat(c.x - CP_System_GetWindowWidth() / 2.0f, c.x + CP_System_GetWindowWidth() / 2.0f);
            bosses[i].y = CP_Random_RangeFloat(c.y - CP_System_GetWindowHeight() / 2.0f, c.y + CP_System_GetWindowHeight() / 2.0f);
          }
          bosses[i].speed = 25;
          bosses[i].cooldown = .2f;
          bosses[i].health = 300;
          break;
        }
      }
    }
  }
}

int dropItem(float loc[], item* items)
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
    {
      items[i].x = loc[0];
      items[i].y = loc[1];
      items[i].type = CP_Random_RangeInt(0, 2);
      switch (items[i].type)
      {
      case 0:
        items[i].containes = CP_Random_RangeInt(40, 90);
        break;
      case 1:
        items[i].containes = CP_Random_RangeInt(1, 4);
        break;
      case 2:
        items[i].containes = CP_Random_RangeInt(1, 5);
        break;
      }
      items[i].active = 1;
      items[i].life = 10;
      return 0;
    }
  }
  return 0;
}