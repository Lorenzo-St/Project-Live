#include "cprocessing.h"
#include "structs.h"
#include "colorStuff.h"
#include "globalData.h"
#include "profileData.h"

#include <stdlib.h>
#include <stdio.h>

bool initialized = false;

CP_Image ground = { 0 };
CP_Image pickup = { 0 };
CP_Image weapons[WEAPON_IMAGES] = { 0 };
CP_Image weaponsGrayScale[WEAPON_IMAGES] = { 0 };
CP_Image ammoImages[AMMO_TYPES] = { 0 };
CP_Image materialsImages[MATERIALS] = { 0 };
CP_Image miscImages[MISC_GRAPHICS] = { 0 };

CP_Image grayScaleify(CP_Image c) 
{
  int size = CP_Image_GetWidth(c) * CP_Image_GetHeight(c);
  FILE* f; 
  fopen_s(&f,"./logs/shadetest.txt", "w");
  CP_Color* newData = (CP_Color*)malloc(size * sizeof(CP_Color));
  CP_Image result = NULL;
  if (newData == NULL)
    return c;
  CP_Image_GetPixelData(c, newData);

  for (int i = 0; i < size; i++) 
  {
    if (newData == NULL)
      break;
    CP_Color Color = *(newData + i);
    float b = (float)Color.b;
    float r = (float)Color.r;
    float g = (float)Color.g;
    float ave = 0;
    if(f != NULL)
      fprintf(f, "Get: %2.2f, %2.2f, %2.2f, %f\n", b, r, g, ave);
    r = r * 0.299f;
    g = g * 0.587f;
    b = b * 0.114f;
    if (f != NULL)
      fprintf(f, "Normalize: %2.2f, %2.2f, %2.2f, %f\n", b, r, g, ave);
    Color.r = (unsigned char)(r + b + g);
    Color.g = (unsigned char)(r + b + g);
    Color.b = (unsigned char)(r + b + g);
    if (f != NULL)
      fprintf(f, "Set: %2.2i, %2.2i, %2.2i, %f\n", Color.b, Color.r, Color.g, ave);
    Color = shadeify(Color, 40);
    *(newData + i) = Color;
  }
  result = CP_Image_CreateFromData(CP_Image_GetWidth(c), CP_Image_GetHeight(c), (unsigned char*)newData);
  free(newData);
  if(f != NULL)
    fclose(f);
  return result;
}

void updateColorMode(void) 
{
  switch (getColorMode()) 
  {
  case LightMode:
    if (ground)
      CP_Image_Free(&ground);
    ground = CP_Image_Load("./Assets/LightModeBG.png");

    break;
  case DarkMode:
    if (ground)
      CP_Image_Free(&ground);
    ground = CP_Image_Load("./Assets/DarkModeBG.png");

    break;
  }
}


int initImages(void) 
{
  CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_NEAREST);
  if (initialized)
    return -1;
  ColorMode c = GetMode();
  

  switch (c) 
  {
  case LightMode:
    ground = CP_Image_Load("./Assets/LightModeBG.png");
    break;
  case DarkMode:
    ground = CP_Image_Load("./Assets/DarkModeBG.png");
    break;
  }
  pickup = CP_Image_Load("./Assets/pickup_image.png");

  /* Weapons */
  weapons[0] = CP_Image_Load("./Assets/pistol_display.png");
  weapons[1] = CP_Image_Load("./Assets/assult_riffle_display.png");
  weapons[2] = CP_Image_Load("./Assets/mini_shotty_display.png");
  weapons[3] = CP_Image_Load("./Assets/rpg_m99lx_display.png");
  
  /* Ammo */
  ammoImages[0] = CP_Image_Load("./Assets/small_ammo_image.png");
  ammoImages[1] = CP_Image_Load("./Assets/medium_ammo_image.png");
  ammoImages[2] = CP_Image_Load("./Assets/large_ammo_image.png");

  /* Materials */
  materialsImages[0] = CP_Image_Load("./Assets/weapon_parts_image.png");
  materialsImages[2] = CP_Image_Load("./Assets/ammo_parts_image.png");
  materialsImages[1] = CP_Image_Load("./Assets/kit_parts_image.png");

  /* Misc Images */
  miscImages[0] = CP_Image_Load("./Assets/health_kit_image.png");

  for (int i = 0; i < WEAPON_IMAGES; i++) 
  {
    weaponsGrayScale[i] =  grayScaleify(weapons[i]);
  }
  
  initialized = true;
  
  return 0;
}

CP_Image returnPickup(void) 
{
  return pickup;
}

CP_Image returnLand(void) 
{
  return ground;
}

CP_Image returnWeaponGS(int id) 
{
  id -= 7 * (id > 1);
  return weaponsGrayScale[id];
}

CP_Image returnAmmo(int id) 
{
  id -= 2;
  return ammoImages[id];
}

CP_Image returnWeapon(int id) 
{
  id -= 7 * (id > 1);
  return weapons[id];
}

CP_Image returnMisc(int id) 
{
  id -= 8;
  return miscImages[id];
}

CP_Image returnMaterials(int id) 
{
  return materialsImages[id - 5];
}

void freeImages(void) 
{
  for (int i = 0; i < WEAPON_IMAGES; i++) 
  {
    CP_Image_Free(&(weapons[i]));
  }

  initialized = false;
}