#include "f2c.h"

/* math.h in vc++ conflicts with f2c.h */ 

extern double sin(), cos(), sinh(), cosh();

void c_cos(complex *r, complex *z)
{
  r->r = (real) (cos(z->r) * cosh(z->i));
  r->i =(real) (- sin(z->r) * sinh(z->i));
}

