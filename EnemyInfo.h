#pragma once
#include "structs.h"

typedef struct EnemyInfo 
{
  const weaponData* weapon;
  float speed;
  float radius;
  int MaxHealth;
  float scaler;
}EnemyInfo;

void loadEnemies();

const EnemyInfo* getEnemy(int id);

void ReleaseEnemiesStorage(void);

size_t getEnemyCount();
