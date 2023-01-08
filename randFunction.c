#include <stdlib.h>

int randNextInt(int lower, int higher)
{
  int next = rand();
  next %= higher - lower;
  next += lower;
  return next;
}

float randNextFloat(float lower, float higher)
{
  float next = (float)rand();
  next /= (float)RAND_MAX;
  next *= (higher - lower);
  next += lower;
  return next;
}