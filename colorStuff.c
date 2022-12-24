#include "cprocessing.h"

CP_Color shadeify(CP_Color c, float percent)
{
  c.b -= (unsigned char)(c.b * (percent / 100));
  c.g -= (unsigned char)(c.g * (percent / 100));
  c.r -= (unsigned char)(c.r * (percent / 100));
  return c;
}

CP_Color tintify(CP_Color c, float percent) 
{
  c.r += (unsigned char)((255 - c.r) * (percent / 100));
  c.g += (unsigned char)((255 - c.g) * (percent / 100));
  c.b += (unsigned char)((255 - c.b) * (percent / 100));
  return c;
}