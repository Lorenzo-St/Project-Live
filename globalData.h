#pragma once
#include "structs.h" 


int    getBulletCount  (void);
float  getTime         (void);
char*  getVersion      (void);
void   setBulletCount  (int);
void   setTime         (float);
void   addTime         (float);
void   setPause        (bool a);
void   setGame         (bool a);
void   setOptions      (bool a);
bool   getOptions      (void);
bool   getPause        (void);
bool   getGame         (void);