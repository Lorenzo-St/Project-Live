#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

InvItem* head = NULL;

int addItem(const char id, char* name, bool stackable,int count, float durability)
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
  newItem->next = head;
  newItem->count = count;
  newItem->itemId = id;
  newItem->name = name;
  newItem->stackable = stackable;
  newItem->durability = durability;

  return 0;
}

InvItem returnStats(char id)
{
  InvItem i = { 0 };

  switch (id) 
  {
  case 0:
    i.itemId = 0;
    i.durability = INFINITY;
    i.name = "Pistol";
    i.next = NULL;
    i.count = 1;
    i.stackable = 0;
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
    if (i == index) 
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

