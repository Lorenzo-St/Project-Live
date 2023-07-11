#include <vector>
#include "playerInfo.h"
#include "playerInput.h"
extern "C"
{
#include "cprocessing.h"
#include "structs.h"
#include "addStuff.h"
#include "shootStuff.h"
#include "playerInv.h"
#include "drawStuff.h"
#include "globalData.h"
#include "gameLoop.h"
#include "mathLib.h"
#include "ItemData.h"
#include "randFunctions.h"
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

  CP_Vector checkLineCollision(CP_Vector* Line1Start, CP_Vector* Line1End, CP_Vector* Line2Start, CP_Vector* Line2End)
  {
    CP_Vector result = *PlayerGetPos(returnPlayer());
    CP_Vector line1;
    Vector2DSub(&line1, Line1End, Line1Start);
    CP_Vector line1Normal = { line1.y, -line1.x };
    Vector2DSub(&line1, Line2End, Line2Start);
    if (Vector2DDot(&line1, &line1Normal) == 0)
      return result;
    if (Vector2DDot(&line1Normal, Line2Start) <= Vector2DDot(&line1Normal, Line1Start) && Vector2DDot(&line1Normal, Line2End) < Vector2DDot(&line1Normal, Line1Start))
      return result;
    if (Vector2DDot(&line1Normal, Line2Start) >= Vector2DDot(&line1Normal, Line1Start) && Vector2DDot(&line1Normal, Line2End) > Vector2DDot(&line1Normal, Line1Start))
      return result;

    float ti = Vector2DDot(&line1Normal, Line1Start) - Vector2DDot(&line1Normal, Line2Start);
    ti /= Vector2DDot(&line1Normal, &line1);

    Vector2DScaleAdd(&result, &line1, Line2Start, ti);
    return result;
  }


  void verifyEnemyCount(enemy* e)
  {
    int count = 0;
    int i = 0;
    while (i < MAX_ENEMIES)
    {
      if ((e + i)->alive)
        count++;
      i++;
    }
    while (getAlive() > count)
      decreaseAlive();
  }




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

  returnStruct checkCircleXRectCollision(float rectX, float rectY, float rectW, float rectH, float circX, float circY, float radius)
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
      return { true, tX, tY, tX2, tY2 };

    return { false };
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
      pressed = CP_Input_KeyTriggered(static_cast<CP_KEY>(key));
      if (pressed)
      {
        if ((*pos) < max)
          dest[(*pos)++] = (char)key;
      }
    }
    for (key = '0'; key <= '9'; key++) // loop through all numbers
    {
      pressed = CP_Input_KeyTriggered(static_cast<CP_KEY>(key));
      if (pressed)
      {
        if ((*pos) < max)
          dest[(*pos)++] = (char)key;
      }
    }
    key = KEY_BACKSPACE;
    pressed = CP_Input_KeyDown(static_cast<CP_KEY>(key));
    if (pressed && *pos > 0)
    {

      dest[--(*pos)] = '\0';
    }
  }

  bool checkKeys(player* pl, float* multiplier, bullet** bullets, bool* InvOpen, bool* wheelOpen, bool* isPaused, int check)
  {
    for (CP_GAMEPAD start = GAMEPAD_DPAD_UP; start <= GAMEPAD_Y; start = static_cast<CP_GAMEPAD>(static_cast<int>(start) + 1))
    {

      if (CP_Input_GamepadTriggered(start))
      {
        switch (start)
        {

          //case GAMEPAD_START:
          //  *isPaused = !(*isPaused);
          //  break;
        case GAMEPAD_B:
          *wheelOpen = false;
          *InvOpen = false;
          break;
        }
      }


    }
    return 0;
  }


  int checkItems(std::vector<item>& items, player* pl, enemy* en, notiString* pickupText)
  {
    const InvItem* j = NULL;
    int count = 1;
    if (items.size() == 0)
      return 0;
    CP_Vector pos = *PlayerGetPos(pl);
    float rad = PlayerGetRadius(pl);

    for (auto& item: items)
    {  
      if (item.active == false)
        continue;
      float distance = sqrtf((item.x - pos.x) * (item.x - pos.x) + (item.y - pos.y) * (item.y - pos.y));
      if (distance < rad + item.radius)
      {
        const InvItem* newest = getItem(item.id);
        if (newest->stackable)
          count = randNextInt(40, 120);
        j = addItem(item.id, count);
        addPickup(&item, j, pickupText, count);
        item.active = false;
      }
      item.life -= CP_System_GetDt();
      if (item.life <= 0)
        item.active = false;

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
    {
      player* p = va_arg(v, player*);
      returnStruct rc;
      CP_Vector pos = *PlayerGetPos(p);
      for (int i = 0; i < buil; i++)
      {
        building* b = buildings + i;
        if (b->w == 0 || b->h == 0)
          continue;
        rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, pos.x, pos.y, PlayerGetRadius(p));
        if (rc.value)
          return 1;

      }
    }
      break;
    case 1:;
    {
      enemy* e = va_arg(v, enemy*);
      returnStruct rc;
      for (int i = 0; i < buil; i++)
      {
        building* b = buildings + i;
        if (b->w == 0 || b->h == 0)
          continue;
        if (b->x - e->pos.x > SCREEN_WIDTH / 2.0f || b->y - e->pos.y > SCREEN_HEIGHT / 2.0f)
          continue;
        rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, e->pos.x, e->pos.y, e->radius / 2.0f);
        if (rc.value)
          return 1;

      }
    }
      break;
    }
    return 0;
  }

  void CheckCollsionWithBuilding(building* b, float x, float y, float r)
  {
    const float radConvert = 3.14159f / 180.0f;
    float dorX = roundFloat(cosf((b->doorSide * 90.0f) * radConvert));
    float dorY = roundFloat(sinf((b->doorSide * 90.0f) * -radConvert));
    // Get both sides of door positioning and account for rotation
    float doorLeft = (b->x + ((b->w / 2.0f) * dorX));
    float doorRight = (b->x + ((b->w / 2.0f) * dorX));
    float doorBottom = (b->y + ((b->h / 2.0f) * dorY));
    float doorTop = (b->y + ((b->h / 2.0f) * dorY));

    doorLeft -= (b->w * .320f * fabsf(dorY));
    doorRight += (b->w * .320f * fabsf(dorY));
    doorTop += (b->h * .320f * fabsf(dorX));
    doorBottom -= (b->h * .320f * fabsf(dorX));

    float width = b->w * .05f + (b->w * .355f * fabsf(dorY));
    float height = b->h * .05f + (b->h * .355f * fabsf(dorX));


    wallsChecks[0] = checkCircleXRectCollision(doorLeft, doorTop, width, height, x, y, r);

    wallsChecks[1] = checkCircleXRectCollision(doorRight, doorBottom, width, height, x, y, r);
    float widthMod = (dorY != 0) ? .05f * fabsf(dorY) : 1.0f;
    float heightMod = (dorX != 0) ? .05f * fabsf(dorX) : 1.0f;

    bool xAlligned = (dorY != 0) ? 1 : 0;
    bool yAlligned = (dorX != 0) ? 1 : 0;

    float cXL = b->x - ((b->w / 2.0f) * xAlligned);
    float cYL = b->y - ((b->h / 2.0f) * yAlligned);
    float cXR = b->x + ((b->w / 2.0f) * xAlligned);
    float cYR = b->y + ((b->h / 2.0f) * yAlligned);

    wallsChecks[2] = checkCircleXRectCollision(cXL, cYL, b->w * widthMod, b->h * heightMod, x, y, r);

    wallsChecks[3] = checkCircleXRectCollision(cXR, cYR, b->w * widthMod, b->h * heightMod, x, y, r);
    widthMod = (dorY != 0) ? .05f * fabsf(dorY) : 1.0f;
    heightMod = (dorX != 0) ? .05f * fabsf(dorX) : 1.0f;
    float cX = b->x + ((b->w / 2.0f) * yAlligned * -dorX);
    float cY = b->y - ((b->h / 2.0f) * xAlligned * -dorY);

    wallsChecks[4] = checkCircleXRectCollision(cX, cY, b->w * heightMod, b->h * widthMod, x, y, r);

  }

  int checkAgainstBuilding(building* buildings, int which, ...)
  {
    int buil = grabBuildingNumb();
    va_list v;
    va_start(v, which);
    int result = 0;
    switch (which)
    {
    case 0:; 
    {

      player* p = va_arg(v, player*);
      for (int i = 0; i < buil; i++)
      {
        building* b = buildings + i;
        if (b->w == 0 || b->h == 0)
          continue;
        returnStruct rc;
        CP_Vector pos = *PlayerGetPos(p);
        CP_Vector velo = *PlayerGetVelocity(p);

        rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, pos.x, pos.y, PlayerGetRadius(p));
        if (rc.value == true)
        {
          float distance = sqrtf(rc.x2 * rc.x2 + rc.y2 * rc.y2);
          if (sqrtf(((pos.x + (velo.x)) - rc.x1) * ((pos.x + (velo.x)) - rc.x1) + rc.y2 * rc.y2) < distance)
            result += 1;
          if (sqrtf(((pos.y + (velo.y)) - rc.y1) * ((pos.y + (velo.y)) - rc.y1) + rc.x2 * rc.x2) < distance)
            result += 2;
        }

      }
    }
      break;
    case 1:;
    {
      enemy* e = va_arg(v, enemy*);
      for (int i = 0; i < buil; i++)
      {
        building* b = buildings + i;
        if (b->w == 0 || b->h == 0)
          continue;
        returnStruct rc;

        rc = checkCircleXRectCollision(b->x, b->y, b->w, b->h, e->pos.x, e->pos.y, e->radius / 2.0f);
        if (rc.value == true)
        {
          float distance = sqrtf(rc.x2 * rc.x2 + rc.y2 * rc.y2);
          if (sqrtf(((e->pos.x + e->dir.x * CP_System_GetDt()) - rc.x1) * ((e->pos.x + e->dir.x * CP_System_GetDt() - rc.x1) + rc.y2 * rc.y2)) < distance)
            result += 1;
          if (sqrtf(((e->pos.y + e->dir.y * CP_System_GetDt()) - rc.y1) * ((e->pos.y + e->dir.y * CP_System_GetDt() - rc.y1) + rc.x2 * rc.x2)) < distance)
            result += 2;

        }
      }
    }
      break;
    case 2:; 
    {
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
    }
      break;
    }
    va_end(v);

    if (result > 3)
      result = 3;
    return result;
  }
}
