#include "f2c.h"

extern double sin(), cos(), sinh(), cosh();

void c_sin(complex *r, complex *z)

{
r->r = sin(z->r) * cosh(z->i);
r->i = cos(z->r) * sinh(z->i);
}
