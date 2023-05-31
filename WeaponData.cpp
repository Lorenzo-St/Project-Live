#include <vector>
#include <fstream>
#include <string>

extern "C" 
{
#include "WeaponData.h"
#include "structs.h"

  std::vector<weaponData*> WeaponStorage;

  const weaponData* getWeapon(int id) 
  {
    if (id < 0 || id >= WeaponStorage.size())
      return nullptr;
    return WeaponStorage[id];
  }

  void loadWeapons() 
  {
    std::ifstream read("./Managed/Weapons.dat");
    while (!read.eof())
    {
      std::string buffer;
      std::getline(read, buffer);
      if (buffer == "")
        continue;
      size_t tab = 0;
      int type =  std::stoi(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);

      int damage = std::stoi(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);

      int ammoReserves = std::stoi(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);

      int ammo = std::stoi(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);

      int ammoType = std::stoi(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);

      int pattern = std::stoi(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);

      float bulletSpeed =  static_cast<float>(std::stof(buffer.c_str() + tab));
      tab = buffer.find('\t', tab + 1);

      float attackSpeed = static_cast<float>(std::stof(buffer.c_str() + tab));
      tab = buffer.find('\t', tab + 1);

      float reloadTime = static_cast<float>(std::stof(buffer.c_str() + tab));


      weaponData* e = new weaponData();
      *e = {
        type,
        damage, 
        ammoReserves,
        ammo,
        ammoType,
        pattern,
        bulletSpeed,
        attackSpeed,
        reloadTime,
      };
      WeaponStorage.push_back(e);
    }
  }



  void ReleaseWeaponsStorage(void) 
  {
    for (auto weapon : WeaponStorage) 
    {
      delete weapon;
    }
    WeaponStorage.clear();
  }

}