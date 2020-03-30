#include "divmod.h"
#include <stdio.h>

int div(int a, int b)
{
  return a / b;
}

int mod(int a, int b)
{
  return a - (a / b) * b;
}
