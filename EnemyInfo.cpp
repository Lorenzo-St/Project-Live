#include <vector>
#include <fstream>
#include <string>
extern "C" 
{
#include "EnemyInfo.h"  
#include "structs.h"
#include "WeaponData.h"
  std::vector<EnemyInfo*> Enemies;
  void loadEnemies() 
  {
    std::ifstream read("./Managed/Enemies.dat");
    while (!read.eof()) 
    {
      std::string buffer;
      std::getline(read, buffer);
      size_t tab = 0;
      char Weaponid = static_cast<char>(std::stoi(buffer.c_str() + tab));
      tab = buffer.find('\t', tab + 1);

      float speed = std::stof(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);

      float radius = std::stof(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);
      int MaxHealth = std::stoi(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);
      float scaler = std::stof(buffer.c_str() + tab);
      EnemyInfo* e = new EnemyInfo(); 
      *e = { 
        getWeapon(Weaponid), 
        speed, 
        radius, 
        MaxHealth, 
        scaler
      };
      Enemies.push_back(e);
    }
  }

  size_t getEnemyCount() 
  {
    return Enemies.size() - 1;
  }


  const EnemyInfo* getEnemy(int id) 
  {
    if (id < 0 || id >= Enemies.size())
      return nullptr;
    return Enemies[id];
  }


  void ReleaseEnemiesStorage(void)
  {
    for (auto weapon : Enemies)
    {
      delete weapon;
    }
    Enemies.clear();
  }

}