#pragma once
typedef struct player player;
typedef union CP_Vector CP_Vector;
typedef struct bullet bullet;
typedef struct weaponData weaponData;
typedef struct camera
{
  float x, y;
}camera;

extern "C++"
{

  const CP_Vector* PlayerGetPos(player const* p);
  const CP_Vector* PlayerGetVelocity(player const* p);
  const CP_Vector* PlayerGetDirection(player const* p);

  player* createPlayer(void);
  weaponData* PlayerGetWeapon(player* p);
  int   PlayerGetActiveAmmo(player* pl);
  int   PlayerGetReserveAmmo(player* pl);
  int   PlayerGetHealth(player* pl);
  int   PlayerGetMaxHealth(player* pl);
  void  PlayerUpdateHealth(player& p, float change);
  void  PlayerMove(player& p);
  void  PlayerSetVelocity(player* p, CP_Vector& newVelo);
  void  PlayerSetDirection(player* p, CP_Vector* newDir);
  void  PlayerFire(player* pl, bullet** b);
  void  PlayerSetPosition(player* p, CP_Vector* pos);
  void  PlayerSetInvincible(player* pl, bool value);
  void  PlayerUpdateMaxHealth(player* pl, float update);
  void  PlayerCheckReload(player* pl);
  void  PlayerSetWeapon(player* p, weaponData* w);
  void  PlayerUpdate(player* pl);
  bool  PlayerIsInvincible(player const* p);
  float PlayerGetSpeed(player const* p);
  float PlayerGetRadius(player const* p);
  float PlayerGetRotation(player* pl);
}