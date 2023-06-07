#include <vector>
#include <fstream>
#include <string>
extern "C" 
{
#define CPP
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
      std::vector<specials> spec;
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
      
      tab = buffer.find('\t', tab + 1);
      if (tab != std::string::npos) 
      {
        std::string special = buffer.substr(tab + 1);
        size_t comma = 0;
        spec.push_back(static_cast<specials>(std::stoi(special.c_str() + comma)));
        while (comma = special.find(',', comma + 1),comma != std::string::npos)
        {
          spec.push_back(static_cast<specials>(std::stoi(special.c_str() + comma)));
        }
      }
      EnemyInfo* e = new EnemyInfo(); 
      *e = { 
        getWeapon(Weaponid), 
        speed, 
        radius, 
        MaxHealth, 
        scaler,
        spec
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