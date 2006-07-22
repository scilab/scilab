#include "f2c.h"

extern double exp(), cos(), sin();

void c_exp(complex *r, complex *z)
{
  double expx;
  expx = exp(z->r);
  r->r = (real) (expx * cos(z->i));
  r->i = (real) (expx * sin(z->i));
}
