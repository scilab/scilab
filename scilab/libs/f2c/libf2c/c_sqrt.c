#include "f2c.h"

extern double sqrt(), f__cabs();

void c_sqrt(complex *r, complex *z)
{
double mag, t;

if( (mag = f__cabs(z->r, z->i)) == 0.)
	r->r = r->i = 0.;
else if(z->r > 0)
	{
	t = (real) sqrt(0.5 * (mag + z->r) );
	r->r = (real)t ;
	t = z->i / t;
	r->i = (real) (0.5 * t);
	}
else
	{
	t = sqrt(0.5 * (mag - z->r) );
	if(z->i < 0)
		t = -t;
	r->i = (real) t;
	t = z->i / t;
	r->r = (real) (0.5 * t);
	}
}
