#include "cprocessing.h"
#include "structs.h"
#include "addStuff.h"
#include "shootStuff.h"
#include "playerInv.h"

#include <stdarg.h>
#include <stdio.h>
#include <math.h>

bool checkMouseBoxCollide(float x, float y, float width, float height) 
{
  float topCorner[2] = { x - .5f * width, y + .5f * height };
  float bottomCorner[2] = { x + .5f * width, y - .5f * height };
  float mousePos[2] = { CP_Input_GetMouseX() - .5f * SCREEN_WIDTH, -(CP_Input_GetMouseY() - .5f * SCREEN_HEIGHT) };
  if (mousePos[0] < topCorner[0] 
    || mousePos[0] > bottomCorner[0] 
    || mousePos[1] > topCorner[1] 
    || mousePos[1] < bottomCorner[1]) 
  {
    return 0;
  }
  return 1;
}

bool checkMouseArcCollide(float startAngle, float endAngle, float centerX, float centerY, float radius) 
{
  float mouse[2] = { CP_Input_GetMouseX(), CP_Input_GetMouseY() };
  float angle = atan2f(centerY - mouse[1], centerX - mouse[0]);
  float distance = sqrtf((mouse[0] - centerX) * (mouse[0] - centerX) + (mouse[1] - centerY) * (mouse[1] - centerY));
  angle *= (180.0f / 3.14159f);
  angle += 180.0f;
  if (angle < startAngle || angle > endAngle || distance > radius)
    return false;
  
  return true;
}

bool checkKeys(player *pl, float *multiplier, bullet *bullets, int *playerKeys, bool *InvOpen, bool* wheelOpen,int isPaused, int check)
{
  (*pl).velocity[0] = 0.0f;
  (*pl).velocity[1] = 0.0f;
  int errored = 0;
  float velo = ((*pl).move_speed * CP_System_GetDt());
  if (CP_Input_MouseClicked())
  {
    if ((*pl).weapon == 4)
      playerFire((*pl), bullets);
  }
  if (CP_Input_MouseDown(MOUSE_BUTTON_1))
  {
    if ((*pl).cooldown <= 0 && (*pl).weapon != 4)
    {
      playerFire((*pl), bullets);
      switch ((*pl).weapon)
      {
      case 0:
        if ((*pl).powerUp != 2)
          (*pl).cooldown = .8f;
        else if ((*pl).powerUp == 2)
          (*pl).cooldown = .1f;
        break;
      case 1:
        if ((*pl).powerUp != 2)
          (*pl).cooldown = .1f;
        else if ((*pl).powerUp == 2)
         break;
      case 2:
        if ((*pl).powerUp != 2)
          (*pl).cooldown = 1.3f;
        else if ((*pl).powerUp == 2)
          (*pl).cooldown = .13f;

        break;
      case 3:
        if ((*pl).powerUp != 2)
          (*pl).cooldown = 1.5f;
        else if ((*pl).powerUp == 2)
          (*pl).cooldown = .15f;
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
      case KEY_I:
        pl->powerUp = 5;
        pl->health = 300;
        pl->powerUpTimer = INFINITY;
        break;
      case KEY_Q:
        *wheelOpen = !*wheelOpen;
        break;
      case KEY_E:
        *InvOpen = !*InvOpen;
        break;
      case KEY_LEFT_SHIFT:
        if (*multiplier <= 1)
        {
          *multiplier = 4;
        }
        break;
      case KEY_SPACE:
        if ((*pl).weapon == 4)
          playerFire((*pl), bullets);
        break;
      }
    }
    if (CP_Input_KeyDown(playerKeys[i]))
    {
      switch (playerKeys[i])
      {

      case KEY_W:
        (*pl).velocity[1] += velo;
        break;
      case KEY_A:
        (*pl).velocity[0] += -velo;
        break;
      case KEY_S:
        (*pl).velocity[1] += -velo;
        break;
      case KEY_D:
        (*pl).velocity[0] += velo;
        break;
      case 256:
        if (isPaused)
          isPaused = 0;
        else
          isPaused = 1;
        break;
      case KEY_SPACE:
        if ((*pl).weapon == 4)
          break;
        if ((*pl).cooldown <= 0)
        {
          playerFire((*pl), bullets);
          switch ((*pl).weapon)
          {
          case 0:
            if ((*pl).powerUp != 2)
              (*pl).cooldown = .8f;
            else if ((*pl).powerUp == 2)
              (*pl).cooldown = .1f;
            break;
          case 1:
            if ((*pl).powerUp != 2)
              (*pl).cooldown = .1f;
            else if ((*pl).powerUp == 2)
              (*pl).cooldown = .0f;

            break;
          case 2:
            if ((*pl).powerUp != 2)
              (*pl).cooldown = 1.3f;
            else if ((*pl).powerUp == 2)
              (*pl).cooldown = .13f;

            break;
          case 3:
            if ((*pl).powerUp != 2)
              (*pl).cooldown = 1.5f;
            else if ((*pl).powerUp == 2)
              (*pl).cooldown = .15f;
            break;
          }
        }
        break;
      }
    }
  }
  
  (*pl).x += ((*pl).velocity[0] * *multiplier) * (check == 0 || check == 2 );
  (*pl).y += ((*pl).velocity[1] * *multiplier) * (check == 0 || check == 1 );
  return errored;
}

int checkItems(item *items, player *pl, enemy *en, enemy *bosses, string *pickupText)
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
      continue;
    float distance = sqrtf((items[i].x - (*pl).x) * (items[i].x - (*pl).x) + (items[i].y - (*pl).y) * (items[i].y - (*pl).y));
    if (distance < (*pl).playerRadius + items[i].radius)
    {
      switch (items[i].type)
      {
      case 0:
        (*pl).health += items[i].containes;
        break;
      case 1:;
        unsigned char id = (unsigned char)items->containes;
        int j = addItem(id);
        if (j == -1)
        {
          items[i].active = 0;
          return -1;
        }
          

        break;
      case 2:
        switch (items[i].containes)
        {
        case 1:
          if ((*pl).powerUpTimer <= 0)
          {
            (*pl).powerUp = items[i].containes;
            (*pl).powerUpTimer = 5.0f;
          }
          break;
        case 2:
          if ((*pl).powerUpTimer <= 0)
          {
            (*pl).powerUp = items[i].containes;
            (*pl).powerUpTimer = 5.0f;
          }
          break;
        case 3:
          (*pl).health = 200;
          break;
        case 4:
          for (int k = 0; k < MAX_ENEMIES; k++)
          {
            if (en[k].active == 0)
              continue;
            en[k].active = 0;
            (*pl).kills++;
          }
          for (int k = 0; k < MAX_BOSSESS; k++)
          {
            if (bosses[k].active == 0)
              continue;
            bosses[k].active = 0;
            (*pl).kills++;
          }
          break;
        case 5:
          if ((*pl).powerUpTimer <= 0)
          {

            (*pl).powerUp = items[i].containes;
            (*pl).powerUpTimer = 5.0f;
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

bool checkInsideBuilding(building buildings[NUMBER_OF_BUILDINGS], int which, ...)
{
  va_list v;
  va_start(v, which);
  switch (which) 
  {
  case 0:;
    player* p = va_arg(v, player*);
    for (int i = 0; i < NUMBER_OF_BUILDINGS; i++)
    {
      building* b = buildings + i;
      if (b->xLen == 0 || b->yLen == 0)
        continue;
      if (b->x - p->x > CP_System_GetWindowWidth() / 2.0f || b->y - p->y > CP_System_GetWindowHeight() / 2.0f)
        continue;
      if (p->x < b->x - (b->xLen / 2.0f) ||
         p->x > b->x + (b->xLen / 2.0f) ||
         p->y < b->y - (b->yLen / 2.0f) ||
         p->y > b->y + (b->yLen / 2.0f));
      else
        return 1;
    
    }
    break;
  case 1:;
    enemy* e = va_arg(v, enemy*);
    for (int i = 0; i < NUMBER_OF_BUILDINGS; i++)
    {
      building* b = buildings + i;
      if (b->xLen == 0 || b->yLen == 0)
        continue;
      if (b->x - e->x > CP_System_GetWindowWidth() / 2.0f || b->y - e->y > CP_System_GetWindowHeight() / 2.0f)
        continue;
      if (e->x < b->x - (b->xLen / 2.0f) ||
        e->x > b->x + (b->xLen / 2.0f) ||
        e->y < b->y - (b->yLen / 2.0f) ||
        e->y > b->y + (b->yLen / 2.0f));
      else
        return 1;

    }
    break;
    break;
  }
  return 0;
}

bool checkAgainstBuilding(building buildings[NUMBER_OF_BUILDINGS], int which, ...)
{
  
  va_list v;
  va_start(v, which);
  int result = 0;
  switch (which) 
  {
  case 0:;
    player* p = va_arg(v, player*);
    for (int i = 0; i < NUMBER_OF_BUILDINGS; i++)
    {
      building* b = buildings + i;
      if (b->xLen == 0 || b->yLen == 0)
        continue;
      if (b->x - p->x > CP_System_GetWindowWidth() / 2.0f || b->y - p->y > CP_System_GetWindowHeight() / 2.0f)
        continue;
      float tX = p->x;
      float tY = p->y;
      if (p->x < b->x - (b->xLen / 2.0f))
        tX = b->x - (b->xLen / 2);
      else if (p->x > b->x + (b->xLen / 2))
        tX = b->x + (b->xLen / 2);
      if (p->y < b->y - (b->yLen / 2.0f))
        tY = b->y - (b->yLen / 2.0f);
      else if (p->y > b->y + (b->yLen / 2.0f))
        tY = b->y + (b->yLen / 2.0f);

      float tX2 = p->x - tX;
      float tY2 = p->y - tY;
      float distance = sqrtf(tX2 * tX2 + tY2 * tY2);
      if (distance < p->playerRadius)
      {

        if (sqrtf(((p->x + (p->velocity[0])) - tX) * ((p->x + (p->velocity[0])) - tX) + tY2 * tY2) < distance)
           result += 1;
        if (sqrtf(((p->y + (p->velocity[1])) - tY) * ((p->y + (p->velocity[1])) - tY) + tX2 * tX2) < distance)
          result += 2;
          
      }
    }
    break;
  case 1:;
    enemy* e = va_arg(v, enemy*);
    for (int i = 0; i < NUMBER_OF_BUILDINGS; i++)
    {
      building* b = buildings + i;
      if (b->xLen == 0 || b->yLen == 0)
        continue;
      float tX = e->x;
      float tY = e->y;
      if (e->x < b->x - (b->xLen / 2.0f))
        tX = b->x - (b->xLen / 2);
      else if (e->x > b->x + (b->xLen / 2))
        tX = b->x + (b->xLen / 2);
      if (e->y < b->y - (b->yLen / 2.0f))
        tY = b->y - (b->yLen / 2.0f);
      else if (e->y > b->y + (b->yLen / 2.0f))
        tY = b->y + (b->yLen / 2.0f);

      float tX2 = e->x - tX;
      float tY2 = e->y - tY;
      float distance = sqrtf(tX2 * tX2 + tY2 * tY2);
      if (distance < e->radius/2.0f)
      {
        if (sqrtf(((e->x + e->dir[0] * CP_System_GetDt()) - tX) * ((e->x + e->dir[0] * CP_System_GetDt() - tX) + tY2 * tY2)) < distance)
          result += 1;
        if (sqrtf(((e->y + e->dir[1] * CP_System_GetDt()) - tY) * ((e->y + e->dir[1] * CP_System_GetDt() - tY) + tX2 * tX2)) < distance)
          result += 2;
        
      }
    }
    break;
  case 2:;
    bullet* bl = va_arg(v, bullet*);
    for (int i = 0; i < NUMBER_OF_BUILDINGS; i++)
    {
      building* b = buildings + i;
      if (b->xLen == 0 || b->yLen == 0)
        continue;
      float tX = bl->x;
      float tY = bl->y;
      if (bl->x < b->x - (b->xLen / 2.0f))
        tX = b->x - (b->xLen / 2);
      else if (bl->x > b->x + (b->xLen / 2))
        tX = b->x + (b->xLen / 2);
      if (bl->y < b->y - (b->yLen / 2.0f))
        tY = b->y - (b->yLen / 2.0f);
      else if (bl->y > b->y + (b->yLen / 2.0f))
        tY = b->y + (b->yLen / 2.0f);

      float tX2 = bl->x - tX;
      float tY2 = bl->y - tY;
      float distance = sqrtf(tX2 * tX2 + tY2 * tY2);
      if (distance < bl->radius)
      {
        return 1;
      }
    }
    break;
  }
  va_end(v);
  return result;
}

