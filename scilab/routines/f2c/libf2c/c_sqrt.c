#include "f2c.h"

extern double sqrt(), f__cabs();

void c_sqrt(complex *r, complex *z)
{
double mag, t;

if( (mag = f__cabs(z->r, z->i)) == 0.)
	r->r = r->i = 0.;
else if(z->r > 0)
	{
	r->r = t = sqrt(0.5 * (mag + z->r) );
	t = z->i / t;
	r->i = 0.5 * t;
	}
else
	{
	t = sqrt(0.5 * (mag - z->r) );
	if(z->i < 0)
		t = -t;
	r->i = t;
	t = z->i / t;
	r->r = 0.5 * t;
	}
}
