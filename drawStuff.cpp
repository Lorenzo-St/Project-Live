#include <vector>
#include <string>
#define CPP
extern "C" 
{
#include "WeaponData.h"
#include "ItemData.h"
#include "drawStuff.h"
#include "structs.h"
#include "cprocessing.h"
#include "stdio.h"
#include "checkStuff.h"
#include "globalImages.h"
#include "playerInv.h"
#include "gameLoop.h"
#include "colorStuff.h"
#include "addStuff.h"
#include "globalData.h"
#include "mathLib.h"
#include <math.h>
#include <stdlib.h>

bool showContext = 0;
bool selectedLoop = false;
bool inContext = false;
bool microOpen = false;
bool upgradeMicro = false;
bool curFreed = false;
bool subSub = false;
bool subsubsub = false;

int subNumb = 0;

scrolllable lightWeapons = { 0 };
scrolllable mediumWeapons = { 0 };
scrolllable heavyWeapons = { 0 };

void drawObjectiveBoard(void) 
{
  float width = SCREEN_WIDTH / 10.0f;
  float height = SCREEN_HEIGHT / 4.0f;
  CP_Color c = GRAY_BUT;
  CP_Settings_Fill(c);
  CP_Graphics_DrawRect(width / 2.0f, height / 2.0f, width, height);
  drawWords("Objectives", width / 2.0f, 30, 45 * (SCREEN_WIDTH / 1920.0f), BLACK);
  objective* obs = returnObis();
  drawWords("Primary:", width / 3.0f, 60, 20  * (SCREEN_WIDTH / 1920.0f), BLACK);
  char buffer[10] = { 0 };
  snprintf(buffer, sizeof buffer, "%f", getTime());
  
  for (int i = 0; i < MAX_OBJECTIVES; i++) 
  {
    float y = 80.0f + (30 * i);
    if (y < height)
      drawWords((obs + i)->title, width / 2.0f, y, 30  * (SCREEN_WIDTH / 1920.0f), BLACK);
    else
      break;
    if (i == 0)
      drawWords(buffer, width / 2.0f, y + 20, 20 * (SCREEN_WIDTH / 1920.0f), BLACK);
  }
}

void drawBackGroundLayer(player* p) 
{
  CP_Image c = returnLand();
  float subX = p->pos.x;
  float subY = -p->pos.y;
  float subX2 = subX + SCREEN_WIDTH / 1.0f;
  float subY2 = subY + SCREEN_HEIGHT / 1.0f;

  CP_Image_DrawSubImage(c, SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f, subX, subY, subX2, subY2, 255);

}

void setContexts(bool ya) 
{
  subSub = ya;
  showContext = ya;
}

void initScrollable(void) 
{
  button* current = NULL;

  lightWeapons.buttons = reinterpret_cast<button*>(calloc(LIGHT_WEAPONS, sizeof(button)));
  current = lightWeapons.buttons;
  
  if (lightWeapons.buttons == NULL)
    return;
  
  for (int i = 0; i < LIGHT_WEAPONS; i++) 
  {
    switch (i) 
    {
    case 0:
      snprintf(current->words, sizeof(current->words), "Pistol x1");
      break;
    case 1:
      snprintf(current->words, sizeof(current->words), "Assult Rifle V1 x5");
      break;
    }
    current++;
  }
  
  
  lightWeapons.scrollPosition = 0;
  
  mediumWeapons.buttons = reinterpret_cast<button*>(calloc(MEDIUM_WEAPONS, sizeof(button)));
  current = mediumWeapons.buttons;
  
  if (mediumWeapons.buttons == NULL)
    return;
  
  for (int i = 0; i < MEDIUM_WEAPONS; i++)
  {
    switch (i)
    {
    case 0:
      snprintf(current->words, sizeof(current->words), "Mini Shotty x10");
      break;
    case 1:
      snprintf(current->words, sizeof(current->words), "NAN");
      break;
    }
    current++;
  }
  mediumWeapons.scrollPosition = 0;
  
  heavyWeapons.buttons = reinterpret_cast<button*>(calloc(HEAVY_WEAPONS, sizeof(button)));
  current = heavyWeapons.buttons;
  
  if (heavyWeapons.buttons == NULL)
    return;

  for (int i = 0; i < HEAVY_WEAPONS; i++)
  {
    switch (i)
    {
    case 0:
      snprintf(current->words, sizeof(current->words), "RPG M99LX x20");
      break;
    case 1:
      snprintf(current->words, sizeof(current->words), "NAN");
      break;
    }
    current++;
  }
  heavyWeapons.scrollPosition = 0;
}

void releaseScrollable(void) 
{
  free(lightWeapons.buttons);
  free(mediumWeapons.buttons);
  free(heavyWeapons.buttons);
}

void drawPlayer(player pl, camera c)
{
  CP_Settings_StrokeWeight(1.5f * (SCREEN_WIDTH / 1920.0f));
  CP_Settings_Fill(CP_Color_CreateHex(0x22A3A4FF));
  CP_Graphics_DrawCircle((pl.pos.x - c.x) + (SCREEN_WIDTH / 2.0f), -(pl.pos.y - c.y) + (SCREEN_HEIGHT / 2.0f), pl.playerRadius * 2);
  CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
  CP_Graphics_DrawRectAdvanced(((pl.pos.x - c.x) + ((pl.direction.x * pl.playerRadius) / sqrtf(pl.direction.x * pl.direction.x + pl.direction.y * pl.direction.y))) + (SCREEN_WIDTH / 2.0f), (-(pl.pos.y - c.y) + ((pl.direction.y * pl.playerRadius) / sqrtf(pl.direction.x * pl.direction.x + pl.direction.y * pl.direction.y))) + (SCREEN_HEIGHT / 2.0f), pl.playerRadius / 1.25f, 10 * (SCREEN_WIDTH / 1920.0f), pl.rot, 0.0f);
  
  CP_Settings_Fill(CP_Color_CreateHex(0x832051FF));
  CP_Settings_RectMode(CP_POSITION_CORNER);
  CP_Graphics_DrawRect((pl.pos.x - c.x) + SCREEN_WIDTH / 2.0f - pl.playerRadius, -(pl.pos.y - c.y) + SCREEN_HEIGHT /2.0f + pl.playerRadius * 1.5f, pl.playerRadius * 2, 20.0f * SCREEN_WIDTH / 1920);
  CP_Settings_Fill(CP_Color_CreateHex(0x81a432FF));
  CP_Graphics_DrawRect((pl.pos.x - c.x) + SCREEN_WIDTH / 2.0f - pl.playerRadius, -(pl.pos.y - c.y) + SCREEN_HEIGHT / 2.0f + pl.playerRadius * 1.5f, pl.playerRadius * 2.0f * (pl.health * 1.0f / pl.maxHealth * 1.0f), 20.0f * SCREEN_WIDTH / 1920);
  CP_Settings_RectMode(CP_POSITION_CENTER);

}

void drawDirector(std::vector<enemy> const& e)
{
   CP_Vector playerPos = returnPlayer()->pos;
   CP_Vector topLeft = { 0 };
   CP_Vector botLeft = { 0 };
   CP_Vector botRight = { 0 };
   CP_Vector topRight = { 0 };
   CP_Vector screenCorners[4] = {
  {-SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT / 2.0f},
  { SCREEN_WIDTH / 2.0f,  SCREEN_HEIGHT / 2.0f},
  {-SCREEN_WIDTH / 2.0f, -SCREEN_HEIGHT / 2.0f},
  { SCREEN_WIDTH / 2.0f, -SCREEN_HEIGHT / 2.0f}
  };

  Vector2DAdd(&topLeft ,  &playerPos, &screenCorners[0]);
  Vector2DAdd(&topRight, &playerPos, &screenCorners[1]);
  Vector2DAdd(&botLeft ,  &playerPos, &screenCorners[2]);
  Vector2DAdd(&botRight, &playerPos, &screenCorners[3]);
  int i = 0;
  for(auto &enemy : e)
  {
    if (enemy.alive == false)
    {
      i++;
      continue;
    }
    CP_Settings_Fill(CP_Color_CreateHex(0xE2E14CFF - (0x15F38C00 * enemy.type)));
    CP_Vector pos = enemy.pos;
    CP_Vector interPos = checkLineCollision(&botRight, &topRight, &playerPos, &pos);
    CP_Vector halfScreen = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    if (!Vector2DEquals(&interPos, &playerPos)) 
    {
      Vector2DSub(&interPos, &interPos, &playerPos);
      interPos.y *= -1;
      Vector2DAdd(&interPos, &interPos, &halfScreen);
      CP_Graphics_DrawCircle(interPos.x, interPos.y, 50);
      i++;
      continue;
    }
    interPos = checkLineCollision(&topRight, &topLeft, &playerPos, &pos);
    if (!Vector2DEquals(&interPos, &playerPos))
    {
      Vector2DSub(&interPos, &interPos, &playerPos);
      interPos.y *= -1;
      Vector2DAdd(&interPos, &interPos, &halfScreen);
      CP_Graphics_DrawCircle(interPos.x, interPos.y, 50);
      i++;
      continue;
    }
    interPos = checkLineCollision(&topLeft, &botLeft, &playerPos, &pos);
    if (!Vector2DEquals(&interPos, &playerPos))
    {
      Vector2DSub(&interPos, &interPos, &playerPos);
      interPos.y *= -1;
      Vector2DAdd(&interPos, &interPos, &halfScreen);
      CP_Graphics_DrawCircle(interPos.x, interPos.y, 50);
      i++;
      continue;
    }
    interPos = checkLineCollision(&botLeft, &botRight, &playerPos, &pos);
    if (!Vector2DEquals(&interPos, &playerPos))
    {
      Vector2DSub(&interPos, &interPos, &playerPos);
      interPos.y *= -1;
      Vector2DAdd(&interPos, &interPos, &halfScreen);
      CP_Graphics_DrawCircle(interPos.x, interPos.y, 50);
      i++;

      continue;
    }
    i++;
  }
}

void drawBullets(bullet* bullets, camera C)
{
  ColorMode c = getColorMode();
  if (c == LightMode)
    CP_Settings_Fill(CP_Color_Create(10, 10, 25, 255));
  else if (c == DarkMode)
    CP_Settings_Fill(CP_Color_Create(250, 250, 250, 255));

  CP_Settings_StrokeWeight(0.0f);
  
  while(bullets)
  { 
    CP_Graphics_DrawCircle((bullets->x - C.x) + (SCREEN_WIDTH / 2.0f), -(bullets->y - C.y) + (SCREEN_HEIGHT / 2.0f), bullets->radius);
    bullets = bullets->next;
  }
}

void drawEnemies(std::vector<enemy> const& e, camera C)
{
  CP_Color c = { 0 };
  for(auto const& enemy : e)
  {
    if (enemy.alive == false) 
      continue;
    c = CP_Color_CreateHex(0xE2E14CFF - (0x15F38C00 * enemy.type));
    CP_Settings_Fill(c);
    CP_Graphics_DrawCircle((enemy.pos.x - C.x) + (SCREEN_WIDTH / 2.0f), -(enemy.pos.y - C.y) + (SCREEN_HEIGHT / 2.0f), enemy.radius);
    CP_Settings_RectMode(CP_POSITION_CORNER);
    CP_Settings_Fill(CP_Color_CreateHex(0x832051FF));
    CP_Graphics_DrawRect((enemy.pos.x - C.x) + (SCREEN_WIDTH / 2.0f) - enemy.radius, -(enemy.pos.y - C.y) + (SCREEN_HEIGHT / 2.0f) + enemy.radius, enemy.radius * 2, 20.0f * SCREEN_WIDTH / 1920);
    CP_Settings_Fill(CP_Color_CreateHex(0x81A432FF));
    CP_Graphics_DrawRect((enemy.pos.x - C.x) + (SCREEN_WIDTH / 2.0f) - enemy.radius, -(enemy.pos.y - C.y) + (SCREEN_HEIGHT / 2.0f) + enemy.radius, enemy.radius * 2.0f * (enemy.health * 1.0f / enemy.MaxHealth * 1.0f), 20.0f * SCREEN_WIDTH / 1920);
    CP_Settings_RectMode(CP_POSITION_CENTER);

  }
}

void drawPickupText(notiString* pickupText, camera C)
{
  CP_Settings_TextSize(30  * (SCREEN_WIDTH / 1920.0f));
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
    CP_Font_DrawText(pickupText[i].buffer, (pickupText[i].x - C.x) + (SCREEN_WIDTH / 2.0f), -(pickupText[i].y - C.y) + (SCREEN_HEIGHT / 2.0f));
    pickupText[i].y++;
    pickupText[i].life -= CP_System_GetDt();
  }
}

void drawItems(std::vector<item> const& items, camera C)
{
  CP_Settings_StrokeWeight(0.0f);
  CP_Settings_TextSize(10 * (SCREEN_WIDTH / 1920.0f));
  CP_Image im = returnPickup();
  float imageSize = 30.0f * SCREEN_WIDTH / 1920.0f;
  for (auto const& item : items)
  {
    if (item.active == false)
      continue;
    CP_Image_Draw(im, (item.x - C.x) + (SCREEN_WIDTH / 2.0f), -(item.y - C.y) + (SCREEN_HEIGHT / 2.0f), imageSize, imageSize, 255);
  }

}

void drawBuildings(building *buildings, camera c) 
{
  int buildingNum = grabBuildingNumb();
  CP_Settings_StrokeWeight(0);
  for (int i = 0; i < buildingNum; i++)
  {
    building b = buildings[i];
    if (b.w == 0 || b.h == 0)
      continue;
    if (fabsf(b.x - c.x) > fabsf(SCREEN_WIDTH / 2.0f) + (b.w / 2.0f) || fabsf(b.y - c.y) > fabsf(SCREEN_HEIGHT / 2.0f) + (b.h / 2.0f))
      continue;
    CP_Settings_Fill(CP_Color_Create(10, 10, 10, 255));
    CP_Graphics_DrawRect((b.x - c.x) + (SCREEN_WIDTH / 2.0f), -(b.y - c.y) + (SCREEN_HEIGHT / 2.0f), b.w, b.h);
    

  }
}

void drawMicroWheel(int index) 
{
  float x = SCREEN_WIDTH / 4.0f;
  float y = SCREEN_HEIGHT / 2.0f;
  float radius = SCREEN_WIDTH / 3.0f;
  CP_Color C = WHEEL_BG;
  CP_Settings_Fill(C);
  C.a += 100;
  CP_Settings_Stroke(C);
  CP_Settings_StrokeWeight(6 * (SCREEN_WIDTH / 1920.0f));
  CP_Graphics_DrawEllipseAdvanced(x, y, radius, radius, 0);
  CP_Settings_StrokeWeight(1 * (SCREEN_WIDTH / 1920.0f));
  radius *= .5f;

  for (float angle = -1; angle <= 300; angle += 60)
  {
    int i = (int)((angle + 1) / 60);
    if (checkMouseArcCollide(angle, angle + 61, x, y, radius))
    {
      inContext = true;
      C = WHEEL_FG;
      if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      {
        addToWheel(returnItemAtPos(index), i, index);
        microOpen = false;
      }

    }
    else
      C = WHEEL_BG;
    CP_Settings_Fill(C);
    drawArc(angle, angle + 61, x, y, radius);
  }
  drawWheelImages(x, y, radius);
}

void drawUpgradeMicro(int index, int count)
{
  float x = SCREEN_WIDTH / 4.0f;
  float y = SCREEN_HEIGHT / 2.0f;
  float radius = SCREEN_WIDTH / 3.0f;
  int upgradeCost = 0;
  CP_Color C = WHEEL_BG;
  InvItem* p = NULL;
  CP_Settings_Fill(C);
  C.a += 100;
  CP_Settings_Stroke(C);
  CP_Settings_StrokeWeight(6 * (SCREEN_WIDTH / 1920.0f));
  CP_Graphics_DrawEllipseAdvanced(x, y, radius, radius, 0);
  CP_Settings_StrokeWeight(1 * (SCREEN_WIDTH / 1920.0f));
  radius *= .5f;

  for (float angle = -1; angle <= 300; angle += 60)
  {
    int i = (int)((angle + 1) / 60);
    if (checkMouseArcCollide(angle, angle + 61, x, y, radius))
    {
      inContext = true;
      p = &returnWheel()[i];
      if (p != NULL)
        upgradeCost = p->subID + 1;
      upgradeCost *= upgradeCost;
      C = WHEEL_FG;
      if (count >= upgradeCost && CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      {
        upgradeMicro = false;
        wheelUpgrade(i);
        removeCount(returnItemAtPos(index)->itemId, upgradeCost);
      }

    }
    else
      C = WHEEL_BG;
    CP_Settings_Fill(C);
    drawArc(angle, angle + 61, x, y, radius);
  }
  drawWheelImages(x, y, radius);
}

void checkClick(int click, int numb, int index) 
{
  if (!click)
    return;

  InvItem* item = returnItemAtPos(index);
  weaponData wd = { 0 };

  switch (numb) 
  {
  case 0:
    curFreed = true;
    break;
  case 1:
    microOpen = true;
    break;
  case 2:
    destructWeapon(index);
    inContext = false;
    showContext = false;
    break;
  case 3:;
    wd = *setStats(&wd, (returnWheel())[returnSelected()].weaponId);
    reloadFromStorage(returnSelected());
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 4:
    if (item->count >= SMALL_T_MED_COMB && item->itemId != 4)
    {
      removeCount(item->itemId, SMALL_T_MED_COMB);
      addItem(item->itemId + 1, 5);
    }
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 5:
    subSub = true;
    break;
  case 6:
    upgradeMicro = true;
    inContext = true;
    break;
  case 7:
    upgradeMicro = false;
    subsubsub = true;
    inContext = true;
    break;
  case 8:;
    int count;
    if (item->count >= BASE_KIT_COST)
    {// Craft all
      count = item->count / BASE_KIT_COST;

      removeCount(item->itemId, count * BASE_KIT_COST);
      addItem(8, count);
      subSub = false;
      subsubsub = false;
    }
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 9: // Creaft 1
    if (item->count >= BASE_KIT_COST) 
    {
      removeCount(item->itemId, BASE_KIT_COST);
      addItem(8, 1);
      inContext = false;
      subSub = false;
    }
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 10: // Craft 10
    if (item->count >= BASE_KIT_COST * 10)
    {
      removeCount(item->itemId, BASE_KIT_COST * 10);
      addItem(8, 10);
      inContext = false;
      subSub = false;
    }
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 11: // Craft 25
    if (item->count >= BASE_KIT_COST * 25)
    {
      removeCount(item->itemId, BASE_KIT_COST * 25);
      addItem(8, 25);
      inContext = false;
      subSub = false;
    }
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 12:
    if(item->count >= SMALL_AMMO_COST)
    {
      int counts = item->count / SMALL_AMMO_COST;
      removeCount(item->itemId, SMALL_AMMO_COST * counts);
      addItem(2, 5 * counts );
    }
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 13:
    if (item->count >= MEDIU_AMMO_COST)
    {
      int counts = item->count / MEDIU_AMMO_COST;
      removeCount(item->itemId, MEDIU_AMMO_COST * counts);
      addItem(3, 3 * counts);
     
    }
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 14:
    if (item->count >= LARGE_AMMO_COST)
    {
      int counts = item->count / LARGE_AMMO_COST;
      removeCount(item->itemId, LARGE_AMMO_COST * counts);
      addItem(4, 5 * counts);
      
    }
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  case 15:
    count = item->count * HEALTH_PER_KIT;
    float difference = (float)returnPlayer()->maxHealth - (float)returnPlayer()->health;
    if (returnPlayer()->health >= (float)returnPlayer()->maxHealth)
      return;
    if (count >= difference) 
    {
      returnPlayer()->health = returnPlayer()->maxHealth;
      difference /= HEALTH_PER_KIT;
      difference += 1;
      removeCount(item->itemId, (int)difference);
    }
    else if (difference > count) 
    {
      returnPlayer()->health += count;
      removeCount(item->itemId, item->count);
    }
    subSub = false;
    subsubsub = false;
    upgradeMicro = false;
    microOpen = false;
    showContext = false;
    setInvSelected(-1);
    break;
  }



}

int checkContext(float x, float y, float width, float height) 
{
  CP_Color c = GRAY_BUT;
  if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), width, height))
  {
    c = tintify(c, 30);
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      return 1;
  }
  else
    c = GRAY_BUT;
  CP_Settings_Fill(c);

  return 0;
}

void drawWords(const char* string, float x, float y, float textSize, CP_Color c) 
{
  CP_Settings_TextSize(textSize);
  CP_Settings_Fill(c);
  CP_Font_DrawText(string, x, y);
}

char getWeaponID(int type, int i) 
{
  std::vector<invItem*> const& itemList = getItemList();
  int wantedCount = 0;
  char pos = -1;
  for (auto const& ite : itemList) 
  {
    ++pos;
    if (ite->weaponId < 0)
      continue;

    const weaponData* weapon = getWeapon(ite->weaponId);
    if (weapon->type != type)
      continue;
    if (wantedCount == i)
    {
      return pos;
    }
    ++wantedCount;

  }
  return -1;
}

void drawSubSubContext(int type, float x, float y) 
{
  float width = SCREEN_WIDTH / 10.0f;
  float height = SCREEN_HEIGHT / 5.0f;
  float buttonHeight = height / 5.25f;
  float buttonWidth = width * .9f;
  int buttonsPerScreen = (int)(height/ buttonHeight) - 1;
  int topButton = 0;
  int bottomButton = 0;
  char id = 0;
  CP_Color c = BLACK;

  if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), width, height))
    inContext = true;
  else
    inContext = false;
  c.a = 175;
  CP_Settings_Fill(c);
  CP_Graphics_DrawRect(x, y, width, height);
  y -= buttonHeight * 1.9f;
  int a;
  switch (type) 
  {
  case 1:
    topButton = lightWeapons.scrollPosition;
    bottomButton = topButton + buttonsPerScreen;

    for (int i = topButton; i < bottomButton; i++) 
    {
      id = getWeaponID(type - 1, i);
      if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), buttonWidth, buttonHeight)) 
      {
        c = WHITE;
        int cost = (int)(4.5f * i) + 1;
        int count = returnItemAtPos(returnInvSelected())->count;
        if (count >= cost && CP_Input_MouseTriggered(MOUSE_BUTTON_1)) 
        {
 
          addItem(id, 1);
          int iid = returnItemAtPos(returnInvSelected())->itemId;
          removeCount(iid, cost);
          subSub = false;
          subsubsub = false;
          inContext = false; 
          showContext = false;
        }
        else if (count < cost)
          c = BAD_BUTT;
      }
      else
        c = GRAY_BUT;
      CP_Settings_Fill(c);
      if (id == -1)
        break;
      CP_Graphics_DrawRect(x, y, buttonWidth, buttonHeight);
      drawWords(getName(id)->c_str(), x, y, 25  * (SCREEN_WIDTH / 1920.0f) , BLACK);
      y += (buttonHeight * 1.25f);
    }

    if (LIGHT_WEAPONS < buttonsPerScreen)
      break;
    a = (int)CP_Input_MouseWheel();
    if (a > 0 && lightWeapons.scrollPosition + a >= 0  )
      lightWeapons.scrollPosition -= a;
    else if(a < 0 && lightWeapons.scrollPosition - a <= LIGHT_WEAPONS)
      lightWeapons.scrollPosition -= a;

    if (lightWeapons.scrollPosition < 0)
      lightWeapons.scrollPosition = 0;
    else if (lightWeapons.scrollPosition >= LIGHT_WEAPONS)
      lightWeapons.scrollPosition -= 1;
    break;
  case 2:
    topButton = mediumWeapons.scrollPosition;
    bottomButton = topButton + buttonsPerScreen;

    for (int i = topButton; i < bottomButton; i++)
    {
      id = getWeaponID(type - 1, i);

      if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), buttonWidth, buttonHeight))
      {
        c = WHITE;
        int cost = (int)(9.25f * i) + 10;
        int count = returnItemAtPos(returnInvSelected())->count;
        if (count >= cost && CP_Input_MouseTriggered(MOUSE_BUTTON_1))
        {

          addItem(id, 1);
          int iid = returnItemAtPos(returnInvSelected())->itemId;
          removeCount(iid, cost);
          subSub = false;
          subsubsub = false;
          inContext = false;
          showContext = false;
        }
        else if (count < cost)
          c = BAD_BUTT;
      }
      else
        c = GRAY_BUT;
      CP_Settings_Fill(c);
      if (id == -1)
        break;
      CP_Graphics_DrawRect(x, y, buttonWidth, buttonHeight);
      drawWords(getName(id)->c_str(), x, y, 25  * (SCREEN_WIDTH / 1920.0f), BLACK);
      y += (buttonHeight * 1.25f);
    }

    if (MEDIUM_WEAPONS < buttonsPerScreen)
      break;
    a = (int)CP_Input_MouseWheel();
    if (a > 0 && mediumWeapons.scrollPosition + a >= 0)
      mediumWeapons.scrollPosition -= a;
    else if (a < 0 && mediumWeapons.scrollPosition - a <= MEDIUM_WEAPONS)
      mediumWeapons.scrollPosition -= a;

    if (mediumWeapons.scrollPosition < 0)
      mediumWeapons.scrollPosition = 0;
    else if (mediumWeapons.scrollPosition >= MEDIUM_WEAPONS)
      mediumWeapons.scrollPosition -= 1;
    break;
  case 3:
    topButton = heavyWeapons.scrollPosition;
    bottomButton = topButton + buttonsPerScreen;

    for (int i = topButton; i < bottomButton; i++)
    {
      id = getWeaponID(type - 1, i);

      if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), buttonWidth, buttonHeight))
      {
        c = WHITE;
        int cost = (int)(15.5f * i) + 20;
        int count = returnItemAtPos(returnInvSelected())->count;
        if (count >= cost && CP_Input_MouseTriggered(MOUSE_BUTTON_1))
        {

          addItem(id, 1);
          int iid = returnItemAtPos(returnInvSelected())->itemId;
          removeCount(iid, cost);
          subSub = false;
          subsubsub = false;
          inContext = false;
          showContext = false;
        }
        else if (count < cost)
          c = BAD_BUTT;
      }
      else
        c = GRAY_BUT;
      CP_Settings_Fill(c);
      if (id == -1)
        break;
      CP_Graphics_DrawRect(x, y, buttonWidth, buttonHeight);
      drawWords(getName(id)->c_str(), x, y, 25 * (SCREEN_WIDTH / 1920.0f), BLACK);
      y += (buttonHeight * 1.25f);
    }

    if (HEAVY_WEAPONS < buttonsPerScreen)
      break;
    a = (int)CP_Input_MouseWheel();
    if (a > 0 && heavyWeapons.scrollPosition + a >= 0)
      heavyWeapons.scrollPosition -= a;
    else if (a < 0 && heavyWeapons.scrollPosition - a <= HEAVY_WEAPONS)
      heavyWeapons.scrollPosition -= a;

    if (heavyWeapons.scrollPosition < 0)
      heavyWeapons.scrollPosition = 0;
    else if (heavyWeapons.scrollPosition >= HEAVY_WEAPONS)
      heavyWeapons.scrollPosition -= 1;
    break;
  }
}

void drawSubContext(float baseLoc[2], int index)
{
  float width = SCREEN_WIDTH / 9.0f;
  float height = SCREEN_HEIGHT / 3.5f;
  int id = returnItemAtPos(index)->itemId;
  baseLoc[0] -= width;
  baseLoc[1] += height / 10.0f;
  CP_Color c = BLACK;
  c.a = 200;
  CP_Settings_Fill(c);
  c = GRAY;
  CP_Settings_Stroke(c);
  CP_Settings_StrokeWeight(5  * (SCREEN_WIDTH / 1920.0f));
  CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
  if (checkMouseBoxCollide(baseLoc[0] - SCREEN_WIDTH / 2.0f, -(baseLoc[1] - SCREEN_HEIGHT / 2.0f), width, height))
    inContext = true;
  else
    inContext = false;

  int click;
  switch(id)
  {
  case 5:
    width *= .9f;
    height /= 5;

    baseLoc[1] -= height * 1.875f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("Upgrade", baseLoc[0], baseLoc[1], 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 6, index);
    
    baseLoc[1] += height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("Craft Light", baseLoc[0], baseLoc[1], 40  * (SCREEN_WIDTH / 1920.0f), BLACK);
    if (click)
      subNumb = 1;
    checkClick(click, 7, index);

    baseLoc[1] += height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("Craft Medium", baseLoc[0], baseLoc[1], 35  * (SCREEN_WIDTH / 1920.0f), BLACK);
    if (click)
      subNumb = 2;
    checkClick(click, 7, index);

    baseLoc[1] += height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("Craft Heavy", baseLoc[0], baseLoc[1], 40  * (SCREEN_WIDTH / 1920.0f), BLACK);
    if (click)
      subNumb = 3;
    checkClick(click, 7, index);

    break;
  case 6:
    width *= .9f;
    height /= 5;

    baseLoc[1] -= height * 1.875f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("Create Max", baseLoc[0], baseLoc[1], 40  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 8, index);

    baseLoc[1] += height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("1 Medkits x4", baseLoc[0], baseLoc[1], 30  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 9, index);

    baseLoc[1] += height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("10 Medkits x40", baseLoc[0], baseLoc[1], 30  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 10, index);

    baseLoc[1] += height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("25 Medkits x100", baseLoc[0], baseLoc[1], 30  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 11, index);

    break;
  case 7:
    width *= .9f;
    height /= 4;

    baseLoc[1] -= height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("Light x5 parts", baseLoc[0], baseLoc[1], 30  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 12, index);

    baseLoc[1] += height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("Medium x10 parts", baseLoc[0], baseLoc[1], 30  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 13, index);

    baseLoc[1] += height * 1.25f;
    click = checkContext(baseLoc[0], baseLoc[1], width, height);
    CP_Graphics_DrawRect(baseLoc[0], baseLoc[1], width, height);
    drawWords("Heavy x25 parts", baseLoc[0], baseLoc[1], 30  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 14, index);
    break;
  }

  if (upgradeMicro)
    drawUpgradeMicro(index, returnItemAtPos(index)->count);
} 

void drawContextMenu(float x, float y, int type, int index) 
{
  float width = SCREEN_WIDTH / 9.0f;
  float height = SCREEN_HEIGHT / 4.5f;
  x -= width / 1.5f;
  y += height / 2.0f;
  CP_Color c = BLACK;
  c.a = 200;
  CP_Settings_Fill(c);
  c = GRAY;
  CP_Settings_Stroke(c);
  CP_Settings_StrokeWeight(5 * (SCREEN_WIDTH / 1920.0f));
  CP_Graphics_DrawRect(x, y, width, height);
 
  if (subSub)
  {
    float loc[2] = {x, y};
    drawSubContext(loc , index);
    inContext = true;
  }
  if (subsubsub)
  { 
    drawSubSubContext(subNumb, x - width * 2.05f, SCREEN_HEIGHT / 3.5f);
    inContext = true;
  }

  if(!subSub && ! subsubsub)
  {
    if (checkMouseBoxCollide(x - SCREEN_WIDTH / 2.0f, -(y - SCREEN_HEIGHT / 2.0f), width, height))
      inContext = true;
    else
      inContext = false;
  }
  c = BLACK;
  c.a = 200;
  CP_Settings_Fill(c);
  c = GRAY;
  CP_Settings_Stroke(c);
  CP_Graphics_DrawRect(x, y - height/2.0f * 1.25f, width, height * .25f);
  InvItem* item = returnItemAtPos(index);
  if (item->itemId == -1)
    return;
  if (item->type == 0) 
  {
    char buffer[30] = { 0 };
    snprintf(buffer, sizeof buffer, "%s lvl: %i", item->name, item->subID);
    drawWords(buffer, x, y - height / 2.0f * 1.25f, 30 * (SCREEN_WIDTH / 1920.0f), WHITE);
  }
  else 
  {
    drawWords(item->name, x, y - height / 2.0f * 1.25f, 30 * (SCREEN_WIDTH / 1920.0f), WHITE);
  }
  
  int click = 0;
  CP_Settings_TextSize(50);
  c = GRAY_BUT;
  CP_Settings_Fill(c);
  CP_Settings_StrokeWeight(0);
  switch (type) 
  {
  case 0:

    width *= .9f;
    height /= 4;
    
    y -= height * 1.25f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Drop", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 0, index);

    y += height * 1.25f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Equip", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 1, index);

    y += height * 1.25f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Dismantle", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 2, index);
    break;
  case 1:
    width *= .9f;
    height /= 4;

    y -= height * 1.25f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Drop", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 0, index);

    y += height * 1.25f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Refill", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 3, index);

    y += height * 1.25f;
    if(returnItemAtPos(index)->itemId != 4)
    {
      click = checkContext(x, y, width, height);
      CP_Graphics_DrawRect(x, y, width, height);
      drawWords("Combine", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
      checkClick(click, 4, index);
    }
    else 
    {
      click = checkContext(x, y, width, height);
      CP_Graphics_DrawRect(x, y, width, height);
    }
    break;
  case 2:
    width *= .9f;
    height /= 3;

    y -= height * .65f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Drop", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 0, index);

    y += height * 1.3f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Combine", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 5, index);


    break;
  case 3:
    width *= .9f;
    height /= 3;

    y -= height * .65f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Drop", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 0, index);

    y += height * 1.3f;
    click = checkContext(x, y, width, height);
    CP_Graphics_DrawRect(x, y, width, height);
    drawWords("Use", x, y, 50  * (SCREEN_WIDTH / 1920.0f), BLACK);
    checkClick(click, 15, index);

    break;
  }

  if (microOpen)
    drawMicroWheel(index);
}

int drawInventory(std::vector<InvItem> const & head) 
{
  float centerX = SCREEN_WIDTH * (6.5f / 8.0f);
  float centerY = SCREEN_HEIGHT / 2.0f;
  float pannelWidth = SCREEN_WIDTH * (3 / 8.0f);
  float pannelHeight = SCREEN_HEIGHT * 1.0f;
  float itemWidth = pannelWidth / ((ITEMS_PER_ROW));
  float itemHeight = itemWidth;
  float itemX = centerX - (pannelWidth/2) + itemWidth;
  float itemY = SCREEN_HEIGHT / 6.0f;
  int row = 0;
  int h = 0;

  CP_Settings_Fill(INVENTORY);
  CP_Graphics_DrawRectAdvanced(centerX, centerY, pannelWidth, pannelHeight, 0, 20.0f);
  CP_Settings_Fill(BLACK);
  CP_Settings_TextSize(90  * (SCREEN_WIDTH / 1920.0f));
  CP_Font_DrawText("Inventory", centerX, itemHeight/1.5f);
  float conPos[2] = { 0 };
  int conData[2] = { 0 };
  int i = 0;
  for(auto const& item : head)
  {
    selectedLoop = false;
    curFreed = false;
    float pos[2] = { itemX + (itemWidth * 1.15f * i) ,itemY + (itemHeight * 1.25f * row) };
    CP_Color c = INV_ITEM;
    if (checkMouseBoxCollide(pos[0] - SCREEN_WIDTH * .5f, -(pos[1] - SCREEN_HEIGHT * .5f), itemWidth, itemHeight))
    {
      if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
      {
        setInvSelected(h);
        subSub = false;
        subsubsub = false;
        upgradeMicro = false;
        microOpen = false;
        showContext = true;
        selectedLoop = true;
      }

      c = WHITE;
    }

    if (returnInvSelected() == h)
    {
      c = shadeify(c, 30);
    }
    CP_Settings_Fill(c);
    CP_Graphics_DrawRectAdvanced(pos[0], pos[1], itemWidth, itemHeight, 0, 20);
    c = BLACK;
    CP_Settings_Fill(c);

    CP_Image img = NULL;
    float angle = 0;
    switch (item.itemId)
    {
    case 2:
      retAmmo()->lightStorage = item.count;
      break;
    case 3:
      retAmmo()->mediumStorage = item.count;
      break;
    case 4:
      retAmmo()->heavyStorage = item.count;
      break;
    }
    switch (item.type)
    {
    case 0:
      img = returnWeaponGS(item.weaponId);
      angle = -45.0f;
      break;
    case 1:
      img = returnAmmo(item.itemId);
      angle = 0.0f;
      break;
    case 2:
      img = returnMaterials(item.itemId);
      angle = 0.0f;
      break;
    case 3:
      img = returnMisc(item.itemId);
      angle = 0.0f;
      break;
    }
    CP_Image_DrawAdvanced(img, pos[0], pos[1], itemWidth * (9.0f / 10.0f), itemHeight * (9.0f / 10.0f), 255, angle);
    if (item.count > 1)
    {
      char buff[10];
      CP_Settings_TextSize(40  * (SCREEN_WIDTH / 1920.0f));
      snprintf(buff, sizeof buff, "%i", item.count);
      CP_Font_DrawText(buff, pos[0] - itemWidth / 4.0f, pos[1] + itemHeight / 3.0f);
    }
    if (returnInvSelected() == h && !selectedLoop)
    {


      if (showContext) 
      {
        conPos[0] = pos[0];
        conPos[1] = pos[1];
        conData[0] = item.type;
        conData[1] = h;
      }


      if (CP_Input_MouseTriggered(MOUSE_BUTTON_1) && !inContext)
      {
        showContext = false;
        subSub = false;
        subsubsub = false;
      }
      if (CP_Input_MouseTriggered(MOUSE_BUTTON_2))
        showContext = true;
    }
    //if (wepDestruct)
    //  return 1;
    //if (curFreed)
    //{
    //  removeItem(h);
    //}
    if (++i >= ITEMS_PER_ROW - 2)
    {
      row++;
      i = 0;
    }
    h++;
  }
  if(showContext)
    drawContextMenu(conPos[0], conPos[1], conData[0], conData[1]);
  if (checkMouseBoxCollide(centerX - SCREEN_WIDTH / 2.0f, -(centerY - SCREEN_HEIGHT / 2.0f), pannelWidth, pannelHeight) || inContext) 
    return 1;

  return 0;
  
}

void drawArc(float startAngle, float endAngle, float x, float y, float radius) 
{
  CP_Graphics_BeginShape();
  CP_Graphics_AddVertex(x, y);
  for (; startAngle < endAngle; startAngle++)
  {
    float x2 = x + radius * cosf(startAngle * 3.1415f / 180.0f);
    float y2 = y + radius * sinf(startAngle * 3.1415f / 180.0f);
    CP_Graphics_AddVertex(x2, y2);
  }
  CP_Graphics_EndShape();
}

void drawWheelImages(float x, float y, float radius)
{
  CP_Settings_ImageMode(CP_POSITION_CENTER);
  float degrees = 30;
  radius *= 2 / 3.0f;
  InvItem* wheel = returnWheel();
  for (int i = 0; i < WHEEL_SIZE; i++, degrees += 60) 
  {
    InvItem curItem = wheel[i];
    if (curItem.itemId == -1) 
    {
      continue;
    }
      
    CP_Image c = returnWeapon(curItem.weaponId);
    float x2 = x + radius * cosf(degrees * 3.1415f / 180.0f);
    float y2 = y + radius * sinf(degrees * 3.1415f / 180.0f);
    float imageSize = x / 5.0f;
    CP_Image_Draw(c, x2, y2, imageSize, imageSize, 255);
    char buffer[30] = { 0 };
    snprintf(buffer, sizeof buffer, "%s lvl: %i", curItem.name, curItem.subID);
    drawWords(buffer, x2, y2 + imageSize / 2.0f, imageSize / 4.0f, BLACK);
  }
}

void drawWheel(player* p)
{
  float x = SCREEN_WIDTH / 2.0f;
  float y = SCREEN_HEIGHT / 2.0f;
  float radius = SCREEN_WIDTH / 2.0f;
  CP_Color C = WHEEL_BG;
  CP_Settings_Fill(C);
  C.a += 100;
  CP_Settings_Stroke(C);
  CP_Settings_StrokeWeight(10  * (SCREEN_WIDTH / 1920.0f));
  CP_Graphics_DrawEllipseAdvanced(x, y, radius, radius, 0);
  CP_Settings_StrokeWeight(2  * (SCREEN_WIDTH / 1920.0f));
  radius *= .5f;
  
  for (float angle = -1; angle <= 300; angle += 60) 
  {
    int i = (int)((angle + 1) / 60);
    if (checkMouseArcCollide(angle, angle + 61, x, y, radius)) 
    {
      C = WHEEL_FG;
      if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) 
      {
        swatchActive(i, p);
        disableWheel();
      }

    }
    else
      C = WHEEL_BG;
 
    if (i == returnSelected())
     C = shadeify(C, 30);
    CP_Settings_Fill(C);
    drawArc(angle, angle + 61, x, y, radius);
  }
  drawWheelImages(x, y, radius);

  
}

void drawAmmo(player* p, bool inv, bool wheel)
{
  char buffer[30] = { 0 };
  int cur = retAmmo()->active[returnSelected()];
  int max = p->weapon->ammo;
  float width = SCREEN_WIDTH / 5.0f;
  float height = SCREEN_HEIGHT / 5.0f;
  snprintf(buffer, sizeof buffer, "%i/%i", cur, max);
  CP_Color c = GRAY;
  c.a = 200;
  CP_Settings_Fill(c);
  c.a = 255;
  CP_Settings_Stroke(c);
  CP_Settings_StrokeWeight(3  * (SCREEN_WIDTH / 1920.0f));
  CP_Graphics_DrawRect(width / 2.0f, SCREEN_HEIGHT - height / 2.0f, width, height);
  CP_Settings_Fill(WHITE);
  CP_Settings_TextSize(100  * (SCREEN_WIDTH / 1920.0f));
  CP_Font_DrawText(buffer, width / 2.0f, SCREEN_HEIGHT - height / 2.0f);
  cur = retAmmo()->reserves[returnSelected()];
  max = p->weapon->ammoReserves;
  snprintf(buffer, sizeof buffer, "%i/%i", cur, max);
  CP_Settings_TextSize(25  * (SCREEN_WIDTH / 1920.0f));
  CP_Font_DrawText(buffer, width / 2.0f, SCREEN_HEIGHT - height / 6.0f);
  if ((returnWheel())[returnSelected()].durability != INFINITY) 
  {
    snprintf(buffer, sizeof buffer, "%3.0f", (returnWheel())[returnSelected()].durability);
    CP_Font_DrawText(buffer, width / 2.0f, SCREEN_HEIGHT - height + height / 4.0f);
  }
}
}
