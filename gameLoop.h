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
void gameLoopInit(void);
void gameLoopUpdate(void);
void gameLoopExit(void);
camera *retCam();


