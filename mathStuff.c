int nthPower(float input, int  pow) 
{
  while (--pow > 0) 
  {
    input *= input;
  }
  return (int)input;
}