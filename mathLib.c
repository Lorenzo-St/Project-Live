float roundFloat(float x)
{
  if (x < 0)
  {
    x -= .5f;
    x = (float)((int)x);
  }
  else
  {
    x += .5f;
    x = (float)((int)x);
  }
  return x;
}