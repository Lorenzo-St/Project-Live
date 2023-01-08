#pragma once
#include "structs.h"

int         loadWorld          (char* worldName);
int         getNumOfBuildings  (void);
int         generateWorld      (void);
building*   returnBuildings    (void);

