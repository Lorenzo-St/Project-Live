#include "structs.h"
#include "playerInv.h"
#include "gameLoop.h"
#include "mathStuff.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static InvItem*  invhead = NULL;
InvItem* (itemWheel[WHEEL_SIZE]) = { NULL };
wheelAmmo ammo = { 0 };
int selected = -1;
int invSelected = -1;

void resetHead(void) 
{
  invhead = NULL;
}

void initializeAmmo(player* p) 
{
  ammo.active[selected] = p->weapon->ammo;
  ammo.reserves[selected] = p->weapon->ammoReserves;
  ammo.lightStorage = 200;
}

void wheelUpgrade(int index) 
{
  itemWheel[index]->subID++;

  if (selected == index) 
  {
    weaponData a = *setStats(&a, itemWheel[index]->itemId);

    returnPlayer()->weapon->damage = a.damage += 5 * nthPower(1.25, itemWheel[index]->subID);
    itemWheel[index]->durability += 10;
  }
  else 
  {
    itemWheel[index]->durability += 10;
  }
}

void reloadFromReserves(void) 
{
  InvItem* r = itemWheel[selected];
  weaponData* w = malloc(1 * sizeof(weaponData));
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
  InvItem* r = itemWheel[wheelLoc];
  weaponData* w = malloc(1 * sizeof(weaponData));
  w = setStats(w, r->itemId);

  switch (w->ammoType) 
  {
  case 0:
    if (ammo.lightStorage == 0)
      break;
    if (ammo.lightStorage > w->ammoReserves + w->ammo)
    {
      ammo.active[wheelLoc] = w->ammo;
      ammo.reserves[wheelLoc] = w->ammoReserves;
      ammo.lightStorage -= w->ammo + w->ammoReserves;
      removeCount(2, w->ammo + w->ammoReserves);
    }
    else
    {
      int dif = ammo.lightStorage - w->ammo;
      if (dif < 0)
      {
        ammo.active[wheelLoc] = ammo.lightStorage;
        removeCount(2, ammo.lightStorage);
        ammo.lightStorage = 0;
        
        break;
      }
      else 
      {
        ammo.active[wheelLoc] = w->ammo;
        ammo.reserves[wheelLoc] = dif;
        ammo.lightStorage = 0;
        removeCount(2, dif);
      }
    }
    break;
  case 1:
    if (ammo.mediumStorage == 0)
      break;
    if (ammo.mediumStorage > w->ammoReserves + w->ammo)
    {
      ammo.active[wheelLoc] = w->ammo;
      ammo.reserves[wheelLoc] = w->ammoReserves;
      ammo.mediumStorage -= w->ammo + w->ammoReserves;
      removeCount(3, w->ammo + w->ammoReserves);
    }
    else
    {
      int dif = ammo.mediumStorage - w->ammo;
      if (dif < 0)
      {
        ammo.active[wheelLoc] = ammo.mediumStorage;
        removeCount(3, ammo.mediumStorage);
        ammo.mediumStorage = 0;

        break;
      }
      else
      {
        ammo.active[wheelLoc] = w->ammo;
        ammo.reserves[wheelLoc] = dif;
        ammo.mediumStorage = 0;
        removeCount(3, dif);
      }
    }
    break;
  case 2:
    if (ammo.heavyStorage == 0)
      break;
    if (ammo.heavyStorage > w->ammoReserves + w->ammo)
    {
      ammo.active[wheelLoc] = w->ammo;
      ammo.reserves[wheelLoc] = w->ammoReserves;
      ammo.heavyStorage -= w->ammo + w->ammoReserves;
      removeCount(4, w->ammo + w->ammoReserves);
    }
    else
    {
      int dif = ammo.heavyStorage - w->ammo;
      if (dif < 0)
      {
        ammo.active[wheelLoc] = ammo.heavyStorage;
        removeCount(4, ammo.heavyStorage);
        ammo.heavyStorage = 0;

        break;
      }
      else
      {
        ammo.active[wheelLoc] = w->ammo;
        ammo.reserves[wheelLoc] = dif;
        ammo.heavyStorage = 0;
        removeCount(4, dif);
      }
    }
    break;
  }
  free(w);
}

void swatchActive(int i, player *pl) 
{
  if (i != selected && itemWheel[i] != NULL)
    selected = i;
  if (pl->weapon != NULL && itemWheel[i] != NULL)
  {
    free(pl->weapon);
    pl->weapon = malloc(1 * sizeof(weaponData));
    pl->weapon = setStats(pl->weapon, itemWheel[i]->itemId);
    pl->weapon->damage += nthPower(1.25, itemWheel[selected]->subID);
  }
  else if (pl->weapon == NULL && itemWheel[i] != NULL) 
  {
    pl->weapon = malloc(1 * sizeof(weaponData));
    pl->weapon = setStats(pl->weapon, itemWheel[i]->itemId);
    pl->weapon->damage += nthPower(1.25, itemWheel[selected]->subID);

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
  if (itemWheel[position] != NULL)
  {
    if (toAdd != invhead)
      itemWheel[position]->next = invhead;
    else
      itemWheel[position]->next = invhead->next;
    invhead = itemWheel[position];
    itemWheel[position] = toAdd;
    toAdd->next = NULL;
    reloadFromStorage(position);
    if (position == selected)
    {
      selected = -1;
      swatchActive(position, returnPlayer());
    }
  }
  else 
  {
    itemWheel[position] = toAdd;
    removeItem(index);
    toAdd->next = NULL;
    reloadFromStorage(position);
    invSelected = -1;
  }

  
  return 0;
}

int removeFromWheel(int position) 
{
  if (itemWheel[position] == NULL)
    return -1;

  itemWheel[position]->next = invhead;
  invhead = itemWheel[position];
  itemWheel[position] = NULL;
  return 0;
}

int addItem(const char id, const int count)
{
  InvItem* newItem = invhead;
  while (newItem)
  {
    if (newItem->stackable && newItem->itemId == id)
    {
      newItem->count += count;
      return 1;
    }
    newItem = newItem->next;
  }
  if (invSelected != -1)
    invSelected++;
  newItem = (InvItem*)malloc(1 * sizeof(InvItem));
  if (newItem == NULL)
    return -1;
  (*newItem) = returnStats(id);
  if (newItem->itemId == -1)
    return -1;
  newItem->next = invhead;
  invhead = newItem;
  if (count != 1)
    newItem->count = count;
  return 0;
}

int destructWeapon(InvItem* wep, int index) 
{
  weaponData wd = *setStats(&wd, wep->itemId);
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
  int a = addItem(5, count);
  if (a == 1) 
  {
    InvItem* l = returnItemAtPos(index);
    removeItem(index);
    freeItem(l);
  }
  else if (a == 0) 
  {
    InvItem* l = returnItemAtPos(index + 1);
    removeItem(index + 1);
    freeItem(l);
  }
    
  invSelected = -1;
  return count;
}

int removeCount(int id, int count) 
{
  InvItem* current = invhead;
  InvItem* last = invhead;
  if (current == NULL)
    return -1;
  int i = 0;
  while (current) 
  {
    if (current->itemId == id) 
    {
      current->count -= count;
      if (current->count <= 0)
      {
        
        InvItem* this = current;
        if (this == invhead)
          invhead = current->next;
        last->next = current->next;
        current = current->next;
        freeItem(this);
        return 1;
      }
      break;
    }
    i++;
    current = current->next;
  }
  return 0;
}

int removeItem(int index) 
{
  InvItem* current = invhead;
  InvItem* previos = invhead;
  if (current == NULL)
    return -1;
  int i = 0; 
  while (current)
  {
    if (i++ == index)
    {
      if (current == invhead)
      {
        invhead = current->next;
      }
      else
      {
        previos->next = current->next;
      }
      break;
    }
    previos = current;
    current = current->next;

  }

  return 0;
}

int freeItem(InvItem* toFree) 
{
  if (toFree == NULL)
    return -1;

  free(toFree);
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
    i.next = NULL;
    i.count = 1;
    i.stackable = false;
    break;
  case 1:
    i.itemId = 1;
    i.subID = 0;
    i.durability = 100;
    i.type = 0;
    i.name = "Assult Rifle";
    i.next = NULL;
    i.count = 1;
    i.stackable = false;
    break;
  case 2:
    i.itemId = 2;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 1;
    i.name = "Light Ammo";
    i.next = NULL;
    i.count = 1;
    i.stackable = true;
    break;
  case 3:
    i.itemId = 3;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 1;
    i.name = "Medium Ammo";
    i.next = NULL;
    i.count = 1;
    i.stackable = true;
    break;
  case 4:
    i.itemId = 4;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 1;
    i.name = "Heavy Ammo";
    i.next = NULL;
    i.count = 1;
    i.stackable = true;
    break;
  case 5:
    i.itemId = 5;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 2;
    i.name = "Weapon Parts";
    i.next = NULL;
    i.count = 1;
    i.stackable = true;
    break;
  case 6:
    i.itemId = 6;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 2;
    i.name = "Kit Parts";
    i.next = NULL;
    i.count = 1;
    i.stackable = true;
    break;
  case 7:
    i.itemId = 7;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 2;
    i.name = "Ammo Parts";
    i.next = NULL;
    i.count = 1;
    i.stackable = true;
    break;
  case 8:
    i.itemId = 8;
    i.subID = 0;
    i.durability = INFINITY;
    i.type = 3;
    i.name = "Heakth Kit";
    i.next = NULL;
    i.count = 1;
    i.stackable = true;
    break;
  case 9:
    i.itemId = 9;
    i.subID = 0;
    i.durability = 75;
    i.type = 0;
    i.name = "Mini Shotty";
    i.next = NULL;
    i.count = 1;
    i.stackable = false;
    break;
  case 10:
    i.itemId = 10;
    i.subID = 0;
    i.durability = 20;
    i.type = 0;
    i.name = "RPG M99LX";
    i.next = NULL;
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
  return invhead;
}

InvItem* returnItemAtPos(int i)
{
  InvItem* temp = invhead;
  for (int j = 0; j < i; j++)
  {
    if (temp->next != NULL)
      temp = temp->next;
    else
      break;
  }
  return temp;
}

InvItem* ((*returnWheel(void))[WHEEL_SIZE])
{
  return &itemWheel;
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
