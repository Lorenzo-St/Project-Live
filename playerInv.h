#pragma once
#include "structs.h"
#ifdef CPP
#include <vector>
#endif
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
const InvItem* addItem           (const char id, const int count);
InvItem*     returnItemAtPos     (int);
#ifdef CPP
std::vector<InvItem> const& returnHead(void);
#endif
InvItem*     returnWheel         (void);
wheelAmmo*   retAmmo             (void);
weaponData*  setStats            (weaponData*,int);