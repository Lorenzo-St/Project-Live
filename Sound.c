#include "cprocessing.h"
#include "structs.h"

#define WEAPON_SOUNDS 2

struct WeaponSound 
{
  int type;
  int id;
  CP_Sound sound;
};

static struct WeaponSound weaponSounds[WEAPON_SOUNDS] = { 0 };

void initSounds(void) 
{
  struct WeaponSound ws = {0};
  ws.type = 0;
  ws.id = 0;
  ws.sound = CP_Sound_Load("./Assets/pistol.ogg");
  weaponSounds[0] = ws;
  ws = (struct WeaponSound){ 0 };
  ws.type = 0;
  ws.id = 1;
  ws.sound = CP_Sound_Load("./Assets/AutoRifle.ogg");
  weaponSounds[1] = ws;

}


CP_Sound getWeaponSounds(int type, int id) 
{
  for (int i = 0; i < WEAPON_SOUNDS; i++) 
  {
    if (weaponSounds[i].id == id && weaponSounds[i].type == type)
      return weaponSounds[i].sound;
  }

  return NULL;
}

void ReleaseSounds(void) 
{
  for (int i = 0; i < WEAPON_SOUNDS; i++) 
  {
    if (weaponSounds[i].sound != NULL)
      CP_Sound_Free(&(weaponSounds[i].sound));
  }
}