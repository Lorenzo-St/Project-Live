#pragma once
#include "structs.h" 


int    getBulletCount    (void);
int    grabBuildingNumb  (void);
float  getTime           (void);
char*  getVersion        (void);
char*  getWorldName      (void);
void   setBulletCount    (int);
void   setTime           (float);
void   addTime           (float);
void   setPause          (bool);
void   setGame           (bool);
void   setOptions        (bool);
void   setGenerated      (bool);
void   setWorldName      (char*);
bool   getOptions        (void);
bool   getPause          (void);
bool   getGame           (void);
bool   getGenerated      (void);