//---------------------------------------------------------
// file:	gamestate_template.h
// author: Lorenzo St. Luce
// email:lorenzo.stluce@digipen.edu
//
// brief:	header file for template gamestate
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "structs.h"
typedef struct camera camera;
typedef struct player player;
int         getAlive        (void);
void        gameLoopInit    (void);
void        gameLoopUpdate  (void);
void        gameLoopExit    (void);
void        increaseAlive   (void);
void        decreaseAlive   (void);
void        disableWheel    (void);
bool        returnInvSel    (void);
bool*       retWheel        (void);
bool*       retInventory    (void);
camera*     retCam          (void);
player*     returnPlayer    (void);
objective*  returnObis      (void);
bullet**    retBullets      (void);

