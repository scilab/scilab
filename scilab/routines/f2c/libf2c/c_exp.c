#include "f2c.h"

extern double exp(), cos(), sin();

void c_exp(complex *r, complex *z)
{
  double expx;
  expx = exp(z->r);
  r->r = expx * cos(z->i);
  r->i = expx * sin(z->i);
}
