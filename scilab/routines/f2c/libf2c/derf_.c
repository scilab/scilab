#include "f2c.h"
#include <stdio.h>

#ifdef KR_headers
double erf();
double derf_(x) doublereal *x;
#else
extern double erf(double);
double derf_(doublereal *x)
#endif
{
#ifdef __MSC__ 
fprintf(stderr,"libf2c : No erf \n");
return(0);
#else 
return( erf(*x) );
#endif 
}
