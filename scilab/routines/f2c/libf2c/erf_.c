#include "f2c.h"
#include <stdio.h>

#ifdef KR_headers
double erf();
double erf_(x) real *x;
#else
extern double erf(double);
double erf_(real *x)
#endif
{
#ifdef __MSC__ 
fprintf(stderr,"libf2c : No erf \n");
return(0);
#else 
return( erf(*x) );
#endif 
}
