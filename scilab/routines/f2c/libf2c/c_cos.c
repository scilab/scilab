#include "f2c.h"

/* math.h in vc++ conflicts with f2c.h */ 

extern double sin(), cos(), sinh(), cosh();

void c_cos(complex *r, complex *z)
{
  r->r = cos(z->r) * cosh(z->i);
  r->i = - sin(z->r) * sinh(z->i);
}

