#ifndef DIVMOD_H
#define DIVMOD_H


int div(int a, int b)
{
  return a / b;
}

int mod(int a, int b)
{
  return a - (a / b) * b;
}


#endif