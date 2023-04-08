#include "structs.h"
#include "playerInv.h"
#include "gameLoop.h"
#include "mathStuff.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
static InvItem invhead[MAX_INV_ITEMS] = {0};
InvItem itemWheel[WHEEL_SIZE] = { 0 };
wheelAmmo ammo = { 0 };
int selected = -1;
int invSelected = -1;

void resetHead(void) 
{
  memset(invhead, -1, sizeof(InvItem) * (MAX_INV_ITEMS));

}

void resetWheel(void) 
{
  memset(itemWheel, -1, sizeof(InvItem) * WHEEL_SIZE);
}

void initializeAmmo(player* p) 
{
  ammo.active[selected] = p->weapon->ammo;
  ammo.reserves[selected] = p->weapon->ammoReserves;
  ammo.lightStorage = 200;
}

void wheelUpgrade(int index) 
{
  itemWheel[index].subID++;

  if (selected == index) 
  {
    weaponData a = *setStats(&a, itemWheel[index].itemId);

    returnPlayer()->weapon->damage = a.damage += 5 * nthPower(1.25, itemWheel[index].subID);
    itemWheel[index].durability += 10;
  }
  else 
  {
    itemWheel[index].durability += 10;
  }
}

void reloadFromReserves(void) 
{
  InvItem* r = &itemWheel[selected];
  weaponData* w = calloc(1, sizeof(weaponData));
  w = setStats(w, r->itemId);

  if (ammo.active[selected] > 0)
  {
    int difference = w->ammo - ammo.active[selected];
    int diff = ammo.reserves[selected] - difference;
    if (diff >= 0) 
    {
      ammo.active[selected] = w->ammo;
      ammo.reserves[selected] = diff;
    }
    else 
    {
      ammo.active[selected] += ammo.reserves[selected];
      ammo.reserves[selected] = 0;
    }

  }else if (ammo.reserves[selected] >= w->ammo)
  {
    ammo.active[selected] = w->ammo;
    ammo.reserves[selected] -= w->ammo;
  }
  else if (ammo.reserves[selected] < w->ammo) 
  {
    ammo.active[selected] = ammo.reserves[selected];
    ammo.reserves[selected] = 0;
  }
  free(w);
}

void reloadFromStorage(int wheelLoc)
{
  InvItem* r = &itemWheel[wheelLoc];
  weaponData* w = malloc(1 * sizeof(weaponData));
  w = setStats(w, r->itemId);
  int missingActive = w->ammo - ammo.active[wheelLoc];
  int missingReserv = w->ammoReserves - ammo.reserves[wheelLoc];
  switch (w->ammoType) 
  {
  case 0:
    if (ammo.lightStorage == 0)
      break;
    if (missingActive > 0) 
    {
      if (ammo.lightStorage >= missingActive)
      {
        ammo.active[wheelLoc] = w->ammo;
        ammo.lightStorage -= missingActive;
        removeCount(2, missingActive);
      }
      else 
      {
        ammo.active[wheelLoc] += ammo.lightStorage;
        removeCount(2, ammo.lightStorage);
        ammo.lightStorage -= ammo.lightStorage;

      }
    }
    if (missingReserv > 0)
    {
     if (ammo.lightStorage >= missingReserv)
      {
        ammo.reserves[wheelLoc] = w->ammoReserves;
        ammo.lightStorage -= missingReserv;
        removeCount(2, missingReserv);

      }
      else
      {
        ammo.reserves[wheelLoc] += ammo.lightStorage;
        removeCount(2, ammo.lightStorage);
        ammo.lightStorage -= ammo.lightStorage;

      }
    }
    
    break;
  case 1:
    if (ammo.mediumStorage == 0)
      break;
    if (missingActive > 0)
    {
      if (ammo.mediumStorage >= missingActive)
      {
        ammo.active[wheelLoc] = w->ammo;
        ammo.mediumStorage -= missingActive;
        removeCount(3, missingActive);
      }
      else
      {
        ammo.active[wheelLoc] += ammo.mediumStorage;
        ammo.mediumStorage -= ammo.mediumStorage;
        removeCount(3, ammo.mediumStorage);

      }
    }
    if (missingReserv > 0)
    {
      if (ammo.mediumStorage >= missingReserv)
      {
        ammo.reserves[wheelLoc] = w->ammoReserves;
        ammo.mediumStorage -= missingReserv;
        removeCount(3, missingReserv);

      }
      else
      {
        ammo.reserves[wheelLoc] += ammo.mediumStorage;
        ammo.mediumStorage -= ammo.mediumStorage;
        removeCount(3, ammo.mediumStorage);

      }
    }
    break;
  case 2:
    if (ammo.heavyStorage == 0)
      break;
    if (missingActive > 0)
    {
      if (ammo.heavyStorage >= missingActive)
      {
        ammo.active[wheelLoc] = w->ammo;
        ammo.heavyStorage -= missingActive;
        removeCount(4, missingActive);
      }
      else
      {
        ammo.active[wheelLoc] += ammo.heavyStorage;
        ammo.heavyStorage -= ammo.heavyStorage;
        removeCount(4, ammo.heavyStorage);

      }
    }
    if (missingReserv > 0)
    {
      if (ammo.heavyStorage >= missingReserv)
      {
        ammo.reserves[wheelLoc] = w->ammoReserves;
        ammo.heavyStorage -= missingReserv;
        removeCount(4, missingReserv);

      }
      else
      {
        ammo.reserves[wheelLoc] += ammo.heavyStorage;
        ammo.heavyStorage -= ammo.heavyStorage;
        removeCount(4, ammo.heavyStorage);

      }
    }
    break;
  }
  free(w);
}

void swatchActive(int i, player *pl) 
{
  if (i != selected && itemWheel[i].itemId != -1)
    selected = i;
  if (pl->weapon != NULL && itemWheel[i].itemId != -1)
  {
    free(pl->weapon);
    pl->weapon = malloc(1 * sizeof(weaponData));
    pl->weapon = setStats(pl->weapon, itemWheel[i].itemId);
    pl->weapon->damage += nthPower(1.25, itemWheel[selected].subID);
  }
  else if (pl->weapon == NULL && itemWheel[i].itemId != -1) 
  {
    pl->weapon = malloc(1 * sizeof(weaponData));
    pl->weapon = setStats(pl->weapon, itemWheel[i].itemId);
    pl->weapon->damage += nthPower(1.25, itemWheel[selected].subID);

  }
}

int returnSelected(void) 
{
  return selected;
}

int addToWheel(InvItem* toAdd, int position, int index)
{
  if (toAdd->type != 0)
    return -1;
  // if the wheel item is not illegal
  if (itemWheel[position].itemId != -1)
  {
    // swap item positions
    InvItem temp = *toAdd;
    memcpy_s(toAdd, sizeof(InvItem), &itemWheel[position], sizeof(InvItem));
    memcpy_s(&itemWheel[position], sizeof(InvItem),&temp , sizeof(InvItem));
    if (position == selected)
    {
      selected = -1;
      swatchActive(position, returnPlayer());
    }

  }
  else 
  {
    memcpy_s(&itemWheel[position], sizeof(InvItem), toAdd, sizeof(InvItem));
    invSelected = -1;
  }
  reloadFromStorage(position);
  removeItem(index);

  return 0;
}

int removeFromWheel(int position) 
{
  int i = 0;
  while (i < MAX_INV_ITEMS)
  {
    if (invhead[i].itemId == -1) 
    {
      memcpy_s(&invhead[i], sizeof(InvItem), &itemWheel[position], sizeof(InvItem));
      memset(&itemWheel[position], -1, sizeof(InvItem));
      return 0;
    }
    i++;
  }
  return -1;
}

InvItem* addItem(const char id, const int count)
{
  int i = 0;
  while (i < MAX_INV_ITEMS) 
  {
    if (invhead[i].itemId == id)
    {
      if (invhead[i].stackable == true)
        invhead[i].count += count;
      else
      {
        while (i < MAX_INV_ITEMS && invhead[i].itemId != -1) i++;
        invhead[i] = returnStats(id);

      }
      return &invhead[i];
    }
    
    i++;
  }
  i = 0;
  while (i < MAX_INV_ITEMS) 
  {
    if (invhead[i].itemId == -1)
    {
      invhead[i] = returnStats(id);
      invhead[i].count = count;
      return &invhead[i];
    }
    i++;
  }

  return NULL;
}

int destructWeapon(int index) 
{
  weaponData wd = *setStats(&wd, invhead[index].itemId);
  int count = 0;
  switch (wd.ammoType) 
  {
  case 0:
    count = 3;
    break;
  case 1:
    count = 7;
    break;
  case 2:
    count = 12;
    break;
  }
  removeItem(index);
  addItem(5, count);
  invSelected = -1;
  return count;
}

int removeCount(int id, int count) 
{

  int i = 0;
  while (i < MAX_INV_ITEMS) 
  {
    if (invhead[i].itemId == id) 
    {
      invhead[i].count -= count;
      if (invhead[i].count <= 0)
        removeItem(i);
      return 0;
    }
    i++;
  }
  return -1;
}

int removeItem(int index) 
{
  memset(&invhead[index], -1, sizeof(InvItem));
  return 0;
}

int returnInvSelected(void)
{
  return invSelected;
}

int setInvSelected(int i)
{
  invSelected = i;
  return invSelected;
}

InvItem returnStats(const char id)
{
  InvItem i = { NULL, -1 };

  switch (id)
  {
  case 0:
    i.itemId = 0;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 0;
    i.name = "Pistol";
    i.count = 1;
    i.stackable = false;
    break;
  case 1:
    i.itemId = 1;
    i.subID = 0;
    i.durability = 100;
    i.type = 0;
    i.name = "Assult Rifle";
    i.count = 1;
    i.stackable = false;
    break;
  case 2:
    i.itemId = 2;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 1;
    i.name = "Light Ammo";
    i.count = 1;
    i.stackable = true;
    break;
  case 3:
    i.itemId = 3;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 1;
    i.name = "Medium Ammo";
    i.count = 1;
    i.stackable = true;
    break;
  case 4:
    i.itemId = 4;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 1;
    i.name = "Heavy Ammo";
    i.count = 1;
    i.stackable = true;
    break;
  case 5:
    i.itemId = 5;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 2;
    i.name = "Weapon Parts";
    i.count = 1;
    i.stackable = true;
    break;
  case 6:
    i.itemId = 6;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 2;
    i.name = "Kit Parts";
    i.count = 1;
    i.stackable = true;
    break;
  case 7:
    i.itemId = 7;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 2;
    i.name = "Ammo Parts";
    i.count = 1;
    i.stackable = true;
    break;
  case 8:
    i.itemId = 8;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 3;
    i.name = "Heakth Kit";
    i.count = 1;
    i.stackable = true;
    break;
  case 9:
    i.itemId = 9;
    i.subID = 0;
    i.durability = 75;
    i.type = 0;
    i.name = "Mini Shotty";
    i.count = 1;
    i.stackable = false;
    break;
  case 10:
    i.itemId = 10;
    i.subID = 0;
    i.durability = 20;
    i.type = 0;
    i.name = "RPG M99LX";
    i.count = 1;
    i.stackable = false;
    break;
  default:
    i.itemId = -1;
    return i;
    break;
  }

  return i;
}

InvItem* returnHead(void)
{
  return &invhead[0];
}

InvItem* returnItemAtPos(int i)
{
  return &invhead[i];
}

InvItem *returnWheel(void)
{
  return &itemWheel[0];
}

wheelAmmo* retAmmo(void)
{
  return &ammo;
}

weaponData* setStats(weaponData* w, int id)
{
  switch (id)
  {
  case 0:
    w->pattern = 0;
    w->damage = 25;
    w->attackSpeed = .75f;
    w->ammoType = 0;
    w->ammo = 15;
    w->ammoReserves = 150;
    w->type = 0;
    w->reloadTime = 1.0f;
    w->bulletSpeed = BULLET_SPEED;
    break;
  case 1:
    w->pattern = 0;
    w->damage = 30;
    w->attackSpeed = .2f;
    w->ammoType = 0;
    w->ammo = 50;
    w->ammoReserves = 400;
    w->type = 0;
    w->reloadTime = 1.0f;
    w->bulletSpeed = BULLET_SPEED;
    break;
  case 9:
    w->pattern = 1;
    w->damage = 30;
    w->attackSpeed = 1.5f;
    w->ammoType = 1;
    w->ammo = 3;
    w->ammoReserves = 20;
    w->type = 1;
    w->reloadTime = 1.25f;
    w->bulletSpeed = BULLET_SPEED * 1.25f;
    break;
  case 10:
    w->pattern = 0;
    w->damage = 200;
    w->attackSpeed = 3.0f;
    w->ammoType = 2;
    w->ammo = 1;
    w->ammoReserves = 5;
    w->type = 2;
    w->reloadTime = 1.25f;
    w->bulletSpeed = BULLET_SPEED * 1.5f;
    break;
  }

  return w;
}
