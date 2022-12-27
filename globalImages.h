#pragma once
#include "CProcessing.h"

int       initImages       (void);
void      freeImages       (void);
CP_Image  returnPickup     (void);
CP_Image  returnLand       (void);
CP_Image  returnWeapon     (int id);
CP_Image  returnWeaponGS   (int id);
CP_Image  returnAmmo       (int id);
CP_Image  returnMaterials  (int id);
CP_Image  returnMisc       (int id);
CP_Image  grayScaleify     (CP_Image c);

