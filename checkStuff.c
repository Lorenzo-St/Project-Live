#include "cprocessing.h"
#include "structs.h"
#include "addStuff.h"
#include "shootStuff.h"
#include "playerInv.h"
#include "drawStuff.h"
#include "globalData.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct returnStruct 
{
  bool value;
  float x1;
  float y1;
  float x2;
  float y2;
}returnStruct;

returnStruct wallsChecks[5] = { 0 };

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

returnStruct checkCircleXRectCollision(float rectX, float rectY, float rectW, float rectH, float circX, float circY,float radius) 
{
  float tX = circX;
  float tY = circY;
  if (circX < rectX - (rectW / 2.0f))
    tX = rectX - (rectW / 2);
  else if (circX > rectX + (rectW / 2))
    tX = rectX + (rectW / 2);
  if (circY < rectY - (rectH / 2.0f))
    tY = rectY - (rectH / 2.0f);
  else if (circY > rectY + (rectH / 2.0f))
    tY = rectY + (rectH / 2.0f);

  float tX2 = circX - tX;
  float tY2 = circY - tY;
  float distance = sqrtf(tX2 * tX2 + tY2 * tY2);
  if (distance < radius)
    return (returnStruct) {true, tX, tY, tX2, tY2};

  return (returnStruct) {false};
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

void checkAlphanumericKeys(char* dest, int* pos, int max)
{
  int key;
  bool pressed;
  for (key = 'A'; key <= 'Z'; key++) // loop through all uppercase letters
  { 
    pressed = CP_Input_KeyTriggered(key);
    if (pressed) 
    {
      if((*pos) < max)
        dest[(*pos)++] = (char)key;
    }
  }
  for (key = '0'; key <= '9'; key++) // loop through all numbers
  { 
    pressed = CP_Input_KeyTriggered(key);
    if (pressed) 
    {
      if ((*pos) < max)
        dest[(*pos)++] = (char)key;
    }
  }
  key = KEY_BACKSPACE;
  pressed = CP_Input_KeyDown(key);
  if (pressed) 
  {
    dest[--(*pos)] = '\0';
  }
}

bool checkKeys(player *pl, float *multiplier, bullet **bullets, bool* InvOpen, bool* wheelOpen, int isPaused, int check)
{
  pl->velocity[0] = 0.0f;
  pl->velocity[1] = 0.0f;
  int errored = 0;
  float velo = (pl->move_speed * CP_System_GetDt());
  if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) 
  {
    if (pl->weapon->type == 2) 
    {
      playerFire((*pl), bullets);
    }
  }
  if (CP_Input_MouseDown(MOUSE_BUTTON_1))
  {
    if (pl->cooldown <= 0 && pl->weapon->type != 2)
    {
      playerFire((*pl), bullets);
      pl->cooldown = pl->weapon->attackSpeed;
    }
  }
  for (int i = 0; i < KEY_COUNT; i++)
  {
    if (CP_Input_KeyTriggered(playerKeys[i]))
    {
      switch (playerKeys[i])
      {
      case KEY_R:
        reloadFromReserves();
        pl->weapon->reloadClock = pl->weapon->reloadTime;
        break;
#ifdef _DEBUG
      case KEY_I:

        pl->powerUp = 5;
        pl->health = 300;
        pl->powerUpTimer = INFINITY;
        break;
#endif
      case KEY_Q:
        *wheelOpen = !*wheelOpen;
        *InvOpen = false;
        setContexts(false);
        break;
      case KEY_E:
        if (*InvOpen)
          setContexts(false);
        *InvOpen = !*InvOpen;
        
        *wheelOpen = false;
        break;
      case KEY_LEFT_CONTROL:
       /* FALL THROUGH */
      case KEY_LEFT_SHIFT:
        if (*multiplier <= 1)
        {
          *multiplier = 4;
        }
        break;
      case KEY_SPACE:
        if (pl->weapon->type == 2)
          playerFire((*pl), bullets);
        break;
      }
    }
    if (CP_Input_KeyDown(playerKeys[i]))
    {
      switch (playerKeys[i])
      {
      case KEY_UP:
      case KEY_W:
        pl->velocity[1] += velo;
        break;
      case KEY_LEFT:
      case KEY_A:
        pl->velocity[0] += -velo;
        break;
      case KEY_DOWN:
      case KEY_S:
        pl->velocity[1] += -velo;
        break;
      case KEY_RIGHT:
      case KEY_D:
        pl->velocity[0] += velo;
        break;
      case 256:
        if (isPaused)
          isPaused = 0;
        else
          isPaused = 1;
        break;
      case KEY_SPACE:
        if (pl->weapon->type == 2)
          break;
        if (pl->cooldown <= 0)
        {
          playerFire((*pl), bullets);
          pl->cooldown = pl->weapon->attackSpeed;
        }
        break;
      }
    }
  }
  
  pl->x += (pl->velocity[0] * *multiplier) * (check == 0 || check == 2 );
  pl->y += (pl->velocity[1] * *multiplier) * (check == 0 || check == 1 );
  return errored;
}

int checkItems(item *items, player *pl, enemy *en, notiString *pickupText)
{
  InvItem* j = NULL;
  int count = 0;
  for (int i = 0; i < MAX_DROPS; i++)
  {
    if (items[i].active == 0)
      continue;
    float distance = sqrtf((items[i].x - pl->x) * (items[i].x - pl->x) + (items[i].y - pl->y) * (items[i].y - pl->y));
    unsigned char id;
    if (distance < pl->playerRadius + items[i].radius)
    {
      switch (items[i].type)
      {
      case 0:;
        id = (unsigned char)CP_Random_RangeInt(2, 4);
        j = addItem(id, items[i].containes);
        count = items[i].containes;
        break;
      case 1:;
        id = (unsigned char)items->containes;
        j = addItem(id, 1);
        count = 1;
        if (j == NULL)
        {
          items[i].active = 0;
          return -1;
        }
        break;
      case 2:;
        id = (unsigned char)CP_Random_RangeInt(5, 7);
        j = addItem(id, items[i].containes);
        count = items[i].containes;
        break;
      }
      if(j)
        addPickup(&items[i],j ,pickupText, count);
      items[i].active = 0;
    }
    items[i].life -= CP_System_GetDt();
    if (items[i].life <= 0)
      items[i].active = 0;
  }
  return 0;
}

bool checkInsideBuilding(building* buildings, int which, ...)
{
  int buil = grabBuildingNumb();
  va_list v;
  va_start(v, which);
  switch (which) 
  {
  case 0:;
    player* p = va_arg(v, player*);
    returnStruct rc;
    for (int i = 0; i < buil; i++)
    {
      building* b = buildings + i;
      if (b->w == 0 || b->h == 0)
        continue;
      rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, p->x, p->y, p->playerRadius);
      if (rc.value)
        return 1;
    
    }
    break;
  case 1:;
    enemy* e = va_arg(v, enemy*);
    for (int i = 0; i < buil; i++)
    {
      building* b = buildings + i;
      if (b->w == 0 || b->h == 0)
        continue;
      if (b->x - e->x > SCREEN_WIDTH / 2.0f || b->y - e->y > SCREEN_HEIGHT / 2.0f)
        continue;
      rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, e->x, e->y, e->radius/2.0f);
      if (rc.value)
        return 1;

    }
    break;
  }
  return 0;
}

void CheckCollsionWithBuilding(building* b, float x, float y, float r) 
{
  const float radConvert = 3.14159f / 180.0f;
  float dorX = roundFloat(cosf((b->doorSide * 90.0f) *  radConvert));
  float dorY = roundFloat(sinf((b->doorSide * 90.0f) * -radConvert));
  // Get both sides of door positioning and account for rotation
  float doorLeft   = (b->x + ((b->w / 2.0f) * dorX));
  float doorRight  = (b->x + ((b->w / 2.0f) * dorX));
  float doorBottom = (b->y + ((b->h / 2.0f) * dorY));
  float doorTop    = (b->y + ((b->h / 2.0f) * dorY));

  doorLeft   -= (b->w * .320f * fabsf(dorY));
  doorRight  += (b->w * .320f * fabsf(dorY));
  doorTop    += (b->h * .320f * fabsf(dorX));
  doorBottom -= (b->h * .320f * fabsf(dorX));

  float width  = b->w * .05f + (b->w * .355f * fabsf(dorY));
  float height = b->h * .05f + (b->h * .355f * fabsf(dorX));
  

  wallsChecks[0] = checkCircleXRectCollision(doorLeft, doorTop, width, height, x, y, r);
    
  wallsChecks[1] = checkCircleXRectCollision(doorRight, doorBottom, width, height, x, y, r);
  float widthMod =  (dorY != 0) ? .05f * fabsf(dorY) : 1.0f;
  float heightMod = (dorX != 0) ? .05f * fabsf(dorX) : 1.0f;

  bool xAlligned = (dorY != 0) ? 1 : 0;
  bool yAlligned = (dorX != 0) ? 1 : 0;

  float cXL = b->x - ((b->w / 2.0f) * xAlligned);
  float cYL = b->y - ((b->h / 2.0f) * yAlligned);
  float cXR = b->x + ((b->w / 2.0f) * xAlligned);
  float cYR = b->y + ((b->h / 2.0f) * yAlligned);

  wallsChecks[2] = checkCircleXRectCollision(cXL, cYL, b->w * widthMod, b->h * heightMod, x, y, r);
  
  wallsChecks[3] = checkCircleXRectCollision(cXR, cYR, b->w * widthMod, b->h * heightMod, x, y, r);
  widthMod =  (dorY != 0) ? .05f * fabsf(dorY) : 1.0f;
  heightMod = (dorX != 0) ? .05f * fabsf(dorX) : 1.0f;
  float cX = b->x + ((b->w / 2.0f) * yAlligned * -dorX);
  float cY = b->y - ((b->h / 2.0f) * xAlligned * -dorY);

  wallsChecks[4] = checkCircleXRectCollision(cX, cY, b->w * heightMod, b->h * widthMod, x, y, r);

}

bool checkAgainstBuilding(building* buildings, int which, ...)
{
  int buil = grabBuildingNumb();
  va_list v;
  va_start(v, which);
  int result = 0;
  switch (which)
  {
  case 0:;
    player* p = va_arg(v, player*);
    for (int i = 0; i < buil; i++)
    {
      building* b = buildings + i;
      if (b->w == 0 || b->h == 0)
        continue;
      returnStruct rc;

      rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, p->x, p->y, p->playerRadius);
      if (rc.value == true)
      {
        float distance = sqrtf(rc.x2 * rc.x2 + rc.y2 * rc.y2);
        if (sqrtf(((p->x + (p->velocity[0])) - rc.x1) * ((p->x + (p->velocity[0])) - rc.x1) + rc.y2 * rc.y2) < distance)
          result += 1;
        if (sqrtf(((p->y + (p->velocity[1])) - rc.y1) * ((p->y + (p->velocity[1])) - rc.y1) + rc.x2 * rc.x2) < distance)
          result += 2;
      }

    }
    break;
  case 1:;
    enemy* e = va_arg(v, enemy*);
    for (int i = 0; i < buil; i++)
    {
      building* b = buildings + i;
      if (b->w == 0 || b->h == 0)
        continue;
      returnStruct rc;

      rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, e->x, e->y, e->radius / 2.0f);
      if (rc.value == true)
      {
        float distance = sqrtf(rc.x2 * rc.x2 + rc.y2 * rc.y2);
        if (sqrtf(((e->x + e->dir[0] * CP_System_GetDt()) - rc.x1) * ((e->x + e->dir[0] * CP_System_GetDt() - rc.x1) + rc.y2 * rc.y2)) < distance)
          result += 1;
        if (sqrtf(((e->y + e->dir[1] * CP_System_GetDt()) - rc.y1) * ((e->y + e->dir[1] * CP_System_GetDt() - rc.y1) + rc.x2 * rc.x2)) < distance)
          result += 2;

      }
    }
    break;
  case 2:;
    bullet* bl = va_arg(v, bullet*);
    for (int i = 0; i < buil; i++)
    {
      building* b = buildings + i;
      if (b->w == 0 || b->h == 0)
        continue;
      returnStruct rc;

      rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, bl->x, bl->y, bl->radius / 2.0f);
      if (rc.value == true)
      {
        return 1;
      }


    }
    break;
  }
  va_end(v);

  if (result > 3)
    result = 3;
  return result;
}
