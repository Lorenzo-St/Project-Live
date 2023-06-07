#include <vector>
#include <array>
extern "C"
{
#include "structs.h"
#include "playerInv.h"
#include "gameLoop.h"
#include "mathStuff.h"
#include "ItemData.h"
#include "WeaponData.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
  static std::vector<InvItem> invhead;
  static std::vector<invItem>& items = invhead;
  std::array<InvItem, WHEEL_SIZE> itemWheel = { 0 };
  wheelAmmo ammo = { 0 };
  int selected = -1;
  int invSelected = -1;

  void resetHead(void)
  {
    invhead.clear();
  }

  void resetWheel(void)
  {
    for (auto& item : itemWheel) 
    {
      item = { 0, -1 };
    }
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
      weaponData a = *setStats(&a, itemWheel[index].weaponId);

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
    weaponData* w = reinterpret_cast<weaponData*>(calloc(1, sizeof(weaponData)));
    w = setStats(w, r->weaponId);

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

    }
    else if (ammo.reserves[selected] >= w->ammo)
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
    weaponData* w = reinterpret_cast<weaponData*>(calloc(1, sizeof(weaponData)));
    w = setStats(w, r->weaponId);
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

  void swatchActive(int i, player* pl)
  {
    if (i != selected && itemWheel[i].itemId != -1)
      selected = i;
    if (pl->weapon != NULL && itemWheel[i].itemId != -1)
    {
      free(pl->weapon);
      pl->weapon = reinterpret_cast<weaponData*>(calloc(1 , sizeof(weaponData)));
      pl->weapon = setStats(pl->weapon, itemWheel[i].weaponId);
      pl->weapon->damage += nthPower(1.25, itemWheel[selected].subID);
    }
    else if (pl->weapon == NULL && itemWheel[i].itemId != -1)
    {
      pl->weapon = reinterpret_cast<weaponData*>(calloc(1 , sizeof(weaponData)));
      pl->weapon = setStats(pl->weapon, itemWheel[i].weaponId);
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
      memcpy_s(&itemWheel[position], sizeof(InvItem), &temp, sizeof(InvItem));
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
    removeItem(index);
    reloadFromStorage(position);

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
  
  int removeItem(int index) 
  {
    if (index == invhead.size() - 1) 
    {
      invhead[index].itemId = -1;
      return 0;
    }
    int i = 0;
    for (std::vector<InvItem>::iterator item = invhead.begin(); 
      item < invhead.end(); 
      item++) 
    {
      if (i++ == index) 
      {
        invhead.erase(item);
        break;
      }
    }
    return 0;
  }

  const InvItem* addItem(const char id, const int count)
  {
    
    for (auto &item : invhead) 
    {
      if (item.itemId == id && item.stackable)
      {
        item.count += count;
        return &item;
      }
    }
    InvItem newest = *getItem(id);
    newest.count = count;
    items.push_back(newest);

    return &items[items.size() - 1];
  }

  int destructWeapon(int index)
  {
    weaponData wd = *setStats(&wd, invhead[index].weaponId);
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
    for (auto &item : items)
    {
      if (item.itemId == id) 
      {
        item.count -= count;
        if (item.count <= 0)
          removeItem(i);
      }
      ++i;

    }
    return -1;
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

 

  std::vector<InvItem> const& returnHead(void)
  {
    return invhead;
  }

  InvItem* returnItemAtPos(int i)
  {
    if (i >= invhead.size() || i < 0)
      return NULL;
    return &invhead[i];
  }

  InvItem* returnWheel(void)
  {
    return &itemWheel[0];
  }

  wheelAmmo* retAmmo(void)
  {
    return &ammo;
  }

  weaponData* setStats(weaponData* w, int id)
  {
    *w = *getWeapon(id);
    return w;
  }

  void cleanInventory(void) 
  {
    for (int i = 0;i < invhead.size(); i++) 
    {
      if (invhead[i].itemId == -1)
        invhead.erase(invhead.begin() + i);
    }
    
  }
}