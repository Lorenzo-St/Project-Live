#include <vector>
#include <fstream>
#include <string>
#include <sstream>

extern "C"
{
#include "WeaponData.h"
#include "structs.h"
#include "randFunctions.h"
  std::vector<invItem*> itemsStorage;
  std::vector<std::string*> names;

  char chooseId() 
  {
    return static_cast<char>(randNextInt(0, static_cast<int>(itemsStorage.size() - 1)));
  }

  const invItem* getItem(char id)
  {
    if (id < 0 || id >= itemsStorage.size())
      return nullptr;
    return itemsStorage[id];
  }

  void loadItems()
  {
    std::ifstream read("./Managed/Items.dat");
    while (!read.eof())
    {
      std::string buffer;
      std::getline(read, buffer);
      if (buffer == "")
        continue;
      size_t tab = buffer.find('\t', 0);
      std::string* name = new std::string(buffer.substr(0, tab));
      names.push_back(name);
      char weaponId = static_cast<char>(std::stoi(buffer.c_str() + tab));
      tab = buffer.find('\t', tab + 1);

      char subId = static_cast<char>(std::stoi(buffer.c_str() + tab));
      tab = buffer.find('\t', tab + 1);

      unsigned char type = static_cast<unsigned char>(std::stoi(buffer.c_str() + tab));
      tab = buffer.find('\t', tab + 1);

      bool stackable;
      size_t start = tab;
      tab = buffer.find('\t', tab + 1);
      std::string sub = buffer.substr(start, tab - start);
      std::istringstream(sub) >> std::boolalpha >> stackable;

      int count = std::stoi(buffer.c_str() + tab);
      tab = buffer.find('\t', tab + 1);

      float durability = std::stof(buffer.c_str() + tab);

      invItem* e = new invItem();
      *e = {
        (*names[names.size() - 1]).c_str(),
        static_cast<char>(itemsStorage.size()),
        weaponId,
        subId,
        type,
        stackable,
        count,
        durability
      };
      itemsStorage.push_back(e);
    }
  }



  void ReleaseItemsStorage(void)
  {
    for (auto weapon : itemsStorage)
    {
      delete weapon;
    }
    itemsStorage.clear();
  }

}