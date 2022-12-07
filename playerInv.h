#pragma once
#include "structs.h"

int addItem(const char id);
int removeItem(int index);
int removeFromWheel(int position);
int freeItem(int index);
int addToWheel(InvItem* toAdd, int position, int index);
InvItem returnStats(const char id);
InvItem* returnHead(void);
InvItem* returnWheel(void);
