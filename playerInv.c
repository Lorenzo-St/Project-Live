#include "structs.h"
#include "playerInv.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

InvItem* head = NULL;
InvItem* itemWheel[WHEEL_SIZE] = { NULL };
int selected;

int addToWheel(InvItem* toAdd, int position, int index)
{
  if (toAdd->type != 0)
    return -1;
  if (itemWheel[position] != NULL)
  {
    itemWheel[position]->next = head;
    head = itemWheel[position];
  }
  itemWheel[position] = toAdd;
  toAdd->next = NULL;
  removeItem(index);
  return 0;
}

int removeFromWheel(int position) 
{
  if (itemWheel[position] == NULL)
    return -1;

  itemWheel[position]->next = head;
  head = itemWheel[position];
  itemWheel[position] = NULL;
  return 0;
}

InvItem* returnHead(void) 
{
  return head;
}

InvItem* returnWheel(void)
{
  return itemWheel[0];
}

int addItem(const char id)
{
  InvItem* newItem = head;
  while (newItem)
  {
    if (newItem->stackable && newItem->itemId == id)
    {
      newItem->count++;
      return 1;
    }
    newItem = newItem->next;
  }

  newItem = (InvItem*)malloc(1 * sizeof(InvItem));
  if (newItem == NULL)
    return -1;
  (*newItem) = returnStats(id);
  if (newItem->itemId == -1)
    return -1;
  newItem->next = head;
  head = newItem;

  return 0;
}

InvItem returnStats(const char id)
{
  InvItem i = { NULL, -1 };

  switch (id) 
  {
  case 0:
    i.itemId = 0;
    i.durability = INFINITY;
    i.name = "Pistol";
    i.next = NULL;
    i.count = 1;
    i.stackable = false;
    break;
  case 1:
    i.itemId = 1;
    i.durability = 100;
    i.name = "Assult Rifle";
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

int removeItem(int index) 
{
  InvItem* current = head;
  InvItem* previos = head;
  if (current == NULL)
    return -1;
  int i = 0;
  while (current)
  {
    if (i++ == index)
    {
      if (current == head)
      {
        head = current->next;
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

int freeItem(int index) 
{
  InvItem* current = head;
  InvItem* previos = head;
  if (current == NULL)
    return -1;
  int i = 0;
  while (current) 
  {
    if (i++ == index) 
    {
      if (current == head) 
      {
        head = current->next;
        free(current);
      }
      else 
      {
        previos->next = current->next;
        free(current);
      }
      break;
    }
    previos = current;
    current = current->next;

  }

  return 0;
}

