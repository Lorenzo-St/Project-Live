#pragma once
#include "structs.h"

int          returnSelected      (void);
int          removeItem          (int);
int          removeFromWheel     (int);
int          removeCount         (int,int);
int          addToWheel          (InvItem*,int,int);
int          returnInvSelected   (void);
int          setInvSelected      (int);
int          destructWeapon      (int);
void         resetWheel          (void);
void         resetHead           (void);
void         wheelUpgrade        (int);
void         swatchActive        (int,player*);
void         initializeAmmo      (player*);
void         reloadFromStorage   (int);
void         reloadFromReserves  (void);
InvItem      returnStats         (const char);
InvItem*     returnItemAtPos     (int);
InvItem*     returnHead          (void);
InvItem*     returnWheel         (void);
InvItem*     addItem             (const char, const int);
wheelAmmo*   retAmmo             (void);
weaponData*  setStats            (weaponData*,int);