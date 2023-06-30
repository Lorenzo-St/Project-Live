#include "cprocessing.h"
#include "WeaponData.h"
#include "playerInput.h"
#include <cmath>

extern "C" 
{
#include "drawStuff.h"
#include "worldSystems.h"
#include "gameLoop.h"
#include "Sound.h"
#include "shootStuff.h"
#include "checkStuff.h"
#include "playerInv.h"
}

typedef struct bullet bullet;

typedef struct player
{
  int health;
  int maxHealth;
  float playerRadius;
  float cooldown;
  float move_speed;
  float rot;
  bool Invincible;
  CP_Vector pos;
  CP_Vector direction;
  CP_Vector velocity;
  weaponData* weapon;
}player;

// To subtract health, pass in a negative change value
void PlayerUpdateHealth(player& p, float change) 
{
  p.health += static_cast<int>(change);
}

void PlayerUpdateMaxHealth(player* pl, float update) 
{
  pl->maxHealth += static_cast<int>(update);
}


void PlayerSetInvincible(player* pl, bool value) 
{
  pl->Invincible = value;
}


bool PlayerIsInvincible(player const* p)
{
  return p->Invincible;
}

player* createPlayer(void) 
{
  player* pl = new player();
  pl->pos = { 0 };
  pl->move_speed = 100;
  pl->cooldown = 1.0f;
  pl->weapon = 0;
  pl->health = MAX_HEALTH;
  pl->maxHealth = pl->health;
  pl->playerRadius = 20 * (SCREEN_WIDTH / 1920.0f);
  return pl;
}

CP_Vector& operator+=(CP_Vector& lhs, CP_Vector const& rhs) 
{
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  return lhs;
}

void PlayerMove(player& p) 
{
  p.pos += p.velocity;
}

void PlayerSetVelocity(player* p, CP_Vector& newVelo) 
{
  p->velocity = newVelo;
}

const CP_Vector* PlayerGetPos(player const* p) 
{
  return &p->pos;
}

void PlayerSetPosition(player* p, CP_Vector* pos) 
{
  p->pos = *pos;
}

const CP_Vector* PlayerGetDirection(player const* p)
{
  return &p->direction;
}

int PlayerGetHealth(player* pl) 
{
  return pl->health;
}

float PlayerGetRotation(player* pl) 
{
  return pl->rot;
}


int PlayerGetMaxHealth(player* pl) 
{
  return pl->maxHealth;
}

float PlayerGetSpeed(player const* p) 
{
  return p->move_speed;
}

void PlayerSetDirection(player* p, CP_Vector* newDir) 
{
  p->direction = *newDir;
}

void PlayerFire(player* pl, bullet** b)
{
  if (pl->cooldown <= 0)
  {
    pl->cooldown = pl->weapon->attackSpeed;
    if (pl->weapon->reloadClock > 0)
    {
      return;
    }
    if (returnInvSel())
      return;
    if (retAmmo()->active[returnSelected()]-- <= 0)
    {

      retAmmo()->active[returnSelected()] = 0;
      reloadFromReserves();
      pl->weapon->reloadClock = pl->weapon->reloadTime;
      return;
    }
    CP_Vector direction = pl->direction;
    runWeaponPattern(pl->weapon, const_cast<CP_Vector*>(&pl->pos), &direction, b, 1);

    CP_Sound s = getWeaponSounds(pl->weapon->type, (returnWheel())[returnSelected()].itemId);
    CP_Sound_Play(s);
  }
}

void PlayerReload(player* pl) 
{
  reloadFromReserves();
  reloadFromStorage(returnSelected());
  pl->weapon->reloadClock = pl->weapon->reloadTime;
}

int PlayerGetActiveAmmo(player* pl) 
{
  return pl->weapon->ammo;
}

int PlayerGetReserveAmmo(player* pl)
{
  return pl->weapon->ammoReserves;
}

void PlayerUpdate(player* pl) 
{
  float LstickX = getXRawDirectional(Left);
  float LstickY = getYRawDirectional(Left);
  float xAxis = getXRawDirectional(None);
  float yAxis = getYRawDirectional(None);
  pl->velocity.x = ((LstickX == 0) ? xAxis : LstickX) * pl->move_speed * CP_System_GetDt();
  pl->velocity.y = ((LstickY == 0) ? yAxis : LstickY) * pl->move_speed * CP_System_GetDt();

  static float multiplier = 1;
  if (isPressed(Dash) || isTriggered(Dash))
    if (multiplier <= 1)
      multiplier = 4;

  int check = checkAgainstBuilding(returnBuildings(), 0, pl);

  pl->pos.x += (pl->velocity.x * multiplier) * (check != 1 && check != 3);
  pl->pos.y += (pl->velocity.y * multiplier) * (check != 2 && check != 3);
  if (multiplier > 1)
    multiplier -= CP_System_GetDt() * 5;

  if (multiplier < 1)
    multiplier = 1;
  float mX = getXRawDirectional(Mouse);
  float mY = getYRawDirectional(Mouse);
  float RstickX = getXRawDirectional(Right);
  float RstickY = getYRawDirectional(Right);

  pl->direction.x = (RstickX == 0) ? mX : RstickX;
  pl->direction.y = (RstickY == 0) ? mY : RstickY;

  pl->rot = (float)(atan2(pl->direction.y, pl->direction.x) * (180.0f / 3.14159265f) + 90.0f);
  PlayerCheckReload(pl);
  if (isTriggered(Inventory)) 
  {
    if (*retInventory())
      setContexts(false);
    *retInventory() = !*retInventory();
  }
  if (isTriggered(Wheel)) 
  {
    *retInventory() = false;
    *retWheel() = !*retWheel();
    setContexts(false);
  }

  if (*retInventory() == true) 
  {
    float X = (1.0f * isTriggered(XAxisPos)) + (-1.0f * isTriggered(XAxisNeg));
    float Y = (1.0f * isTriggered(YAxisPos)) + (-1.0f * isTriggered(YAxisNeg));
    moveSelected({ X, Y });
  }

}

const CP_Vector* PlayerGetVelocity(player const* p) 
{
  return &p->velocity;
}

float PlayerGetRadius(player const* p) 
{
  return p->playerRadius;
}

weaponData* PlayerGetWeapon(player* p) 
{
  return p->weapon;
}

void PlayerSetWeapon(player* p, weaponData* w) 
{
  p->weapon = w;
}


void PlayerCheckReload(player* pl) 
{
  if (pl->weapon->reloadClock > 0)
  {
    const float reloadPercent = (pl->weapon->reloadTime - pl->weapon->reloadClock) / pl->weapon->reloadTime;
    for (float i = 0; i < 360 * reloadPercent; i += 60)
    {
      CP_Settings_Fill(CP_Color_CreateHex(0xa8bbd9ff));
      CP_Settings_StrokeWeight(2);
      drawArc(i, i + 61, CP_Input_GetMouseX(), CP_Input_GetMouseY(), 30);
    }
    pl->weapon->reloadClock -= CP_System_GetDt();
  }
  else if (pl->weapon->reloadClock < 0)
    pl->weapon->reloadClock = 0;
}
