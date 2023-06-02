#pragma once
#ifdef CPP
#include <vector>
#endif

#include "structs.h"

  void  initScrollable(void);
  void  releaseScrollable(void);
  void  drawObjectiveBoard(void);
  void  drawBackGroundLayer(player* p);
  void  setContexts(bool ya);
  void  drawPlayer(player pl, camera c);
  void  drawWeapon(int weapon, float powerUpTimer, int powerUp);
  void  drawBullets(bullet* bullets, camera C);
#ifdef CPP
  int   drawInventory(std::vector<InvItem> const& head);
  void  drawEnemies(std::vector<enemy> const& e, camera C);
  void  drawDirector(std::vector<enemy> const& e);
  void  drawItems(std::vector<item> const&, camera C);

#endif
  void  drawPickupText(notiString* pickupText, camera C);
  void  drawBuildings(building* buildings, camera c);
  void  drawWheel(player* p);
  void  drawAmmo(player* p, bool inv, bool wheel);
  void  drawWheelImages(float x, float y, float radius);
  void  drawArc(float startAngle, float endAngle, float x, float y, float radius);
  void  drawWords(const char* string, float x, float y, float textSize, CP_Color c);

