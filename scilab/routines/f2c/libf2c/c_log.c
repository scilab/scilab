#include "f2c.h"
extern double log(), f__cabs(), atan2();

void c_log(complex *r, complex *z)
{
r->i = atan2(z->i, z->r);
r->r = log( f__cabs(z->r, z->i) );
}
