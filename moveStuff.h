#pragma once
#ifdef CPP
#include <vector>

#endif

typedef struct bullet bullet;
typedef struct enemy enemy;
typedef struct building building;
typedef struct player player;
typedef struct item item;
#ifdef CPP
int moveBullets(bullet** bullets, std::vector<enemy>& e, player* pl, item* items, building buildings[]);
int moveEnemies(std::vector<enemy>& e, building* buildings);
#endif
