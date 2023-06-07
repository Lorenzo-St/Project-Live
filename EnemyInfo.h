#pragma once
#include "structs.h"

typedef enum specials 
{
  Rapid,
  Shielded,
  Strong,

}specials;


typedef struct EnemyInfo 
{
  const weaponData* weapon;
  float speed;
  float radius;
  int MaxHealth;
  float scaler;
#ifdef CPP
#include <vector>
  std::vector<specials> stats;
#endif
}EnemyInfo;

void loadEnemies();

const EnemyInfo* getEnemy(int id);

void ReleaseEnemiesStorage(void);

size_t getEnemyCount();

