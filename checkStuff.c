#include "cprocessing.h"
#include "structs.h"
#include "addStuff.h"
#include "shootStuff.h"


int checkKeys(player pl, float multiplier, bullet *bullets, int *playerKeys, int isPaused)
{
  pl.velocity[0] = 0.0f;
  pl.velocity[1] = 0.0f;
  int errored = 0;
  float velo = (pl.move_speed * CP_System_GetDt());
  if (CP_Input_MouseClicked())
  {
    if (pl.weapon == 4)
      playerFire(pl, bullets);
  }
  if (CP_Input_MouseDown(MOUSE_BUTTON_1))
  {
    if (pl.cooldown <= 0 && pl.weapon != 4)
    {
      playerFire(pl, bullets);
      switch (pl.weapon)
      {
      case 0:
        if (pl.powerUp != 2)
          pl.cooldown = .8f;
        else if (pl.powerUp == 2)
          pl.cooldown = .1f;
        break;
      case 1:
        if (pl.powerUp != 2)
          pl.cooldown = .1f;
        else if (pl.powerUp == 2)
          pl.cooldown = .0f;

        break;
      case 2:
        if (pl.powerUp != 2)
          pl.cooldown = 1.3f;
        else if (pl.powerUp == 2)
          pl.cooldown = .13f;

        break;
      case 3:
        if (pl.powerUp != 2)
          pl.cooldown = 1.5f;
        else if (pl.powerUp == 2)
          pl.cooldown = .15f;
        break;
      }
    }
  }
  for (int i = 0; i < KEY_COUNT; i++)
  {
    if (CP_Input_KeyTriggered(playerKeys[i]))
    {
      switch (playerKeys[i])
      {
      case 340:
        if (multiplier <= 1)
        {
          multiplier = 4;
        }
        break;
      case 32:
        if (pl.weapon == 4)
          playerFire(pl, bullets);
        break;
      }
    }
    if (CP_Input_KeyDown(playerKeys[i]))
    {
      switch (playerKeys[i])
      {
      case 87:
        pl.velocity[1] += velo;
        break;
      case 65:
        pl.velocity[0] += -velo;
        break;
      case 83:
        pl.velocity[1] += -velo;
        break;
      case 68:
        pl.velocity[0] += velo;
        break;
      case 256:
        if (isPaused)
          isPaused = 0;
        else
          isPaused = 1;
        break;
      case 32:
        if (pl.weapon == 4)
          break;
        if (pl.cooldown <= 0)
        {
          playerFire(pl, bullets);
          switch (pl.weapon)
          {
          case 0:
            if (pl.powerUp != 2)
              pl.cooldown = .8f;
            else if (pl.powerUp == 2)
              pl.cooldown = .1f;
            break;
          case 1:
            if (pl.powerUp != 2)
              pl.cooldown = .1f;
            else if (pl.powerUp == 2)
              pl.cooldown = .0f;

            break;
          case 2:
            if (pl.powerUp != 2)
              pl.cooldown = 1.3f;
            else if (pl.powerUp == 2)
              pl.cooldown = .13f;

            break;
          case 3:
            if (pl.powerUp != 2)
              pl.cooldown = 1.5f;
            else if (pl.powerUp == 2)
              pl.cooldown = .15f;
            break;
          }
        }
        break;
      }
    }
  }
  if (pl.x + pl.velocity[0] * multiplier < CP_System_GetDisplayWidth() / 2.0f && pl.x + pl.velocity[0] * multiplier > -CP_System_GetDisplayWidth() / 2.0f)
    pl.x += pl.velocity[0] * multiplier;
  if (pl.y + pl.velocity[1] * multiplier < CP_System_GetDisplayHeight() / 2.0f && pl.y + pl.velocity[1] * multiplier > -CP_System_GetDisplayHeight() / 2.0f)
    pl.y += pl.velocity[1] * multiplier;
  return errored;
}


int checkItems(item* items, player pl, enemy* en, enemy* bosses, string *pickupText)
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
      continue;
    float distance = sqrtf((items[i].x - pl.x) * (items[i].x - pl.x) + (items[i].y - pl.y) * (items[i].y - pl.y));
    if (distance < pl.playerRadius + items[i].radius)
    {
      switch (items[i].type)
      {
      case 0:
        pl.health += items[i].containes;
        break;
      case 1:
        pl.weapon = items[i].containes;
        break;
      case 2:
        switch (items[i].containes)
        {
        case 1:
          if (pl.powerUpTimer <= 0)
          {
            pl.powerUp = items[i].containes;
            pl.powerUpTimer = 5.0f;
          }
          break;
        case 2:
          if (pl.powerUpTimer <= 0)
          {
            pl.powerUp = items[i].containes;
            pl.powerUpTimer = 5.0f;
          }
          break;
        case 3:
          pl.health = 200;
          break;
        case 4:
          for (int k = 0; k < MAX_ENEMIES; k++)
          {
            if (en[k].active == 0)
              continue;
            en[k].active = 0;
            pl.kills++;
          }
          for (int k = 0; k < MAX_BOSSESS; k++)
          {
            if (bosses[k].active == 0)
              continue;
            bosses[k].active = 0;
            pl.kills++;
          }
          break;
        case 5:
          if (pl.powerUpTimer <= 0)
          {

            pl.powerUp = items[i].containes;
            pl.powerUpTimer = 5.0f;
          }
          break;
        }

        break;

      }
      addPickup(items[i], pickupText);
      items[i].active = 0;
    }
    items[i].life -= CP_System_GetDt();
    if (items[i].life <= 0)
      items[i].active = 0;
  }
  return 0;
}
