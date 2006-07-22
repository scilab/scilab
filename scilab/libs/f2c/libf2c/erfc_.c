#include "f2c.h"
#include <stdio.h>

#ifdef KR_headers
double erfc();
double erfc_(x) real *x;
#else
extern double erfc(double);
double erfc_(real *x)
#endif
{
#ifdef _MSC_VER 
fprintf(stderr,"libf2c : No erf \n");
return(0);
#else 
return( erfc(*x) );
#endif 
}
