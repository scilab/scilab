#include "f2c.h"
#include <stdio.h>

#ifdef KR_headers
extern double erfc();

double derfc_(x) doublereal *x;
#else
extern double erfc(double);

double derfc_(doublereal *x)
#endif
{
#ifdef __MSC__ 
fprintf(stderr,"libf2c : No erf \n");
return(0);
#else 
return( erfc(*x) );
#endif
}
