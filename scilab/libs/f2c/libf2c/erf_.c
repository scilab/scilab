#include "f2c.h"
#ifdef _MSC_VER
#include <stdio.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#ifndef REAL
#define REAL double
#endif

#ifdef KR_headers
double erf();
REAL erf_(x) real *x;
#else
extern double erf(double);
REAL erf_(real *x)
#endif
{
#ifdef _MSC_VER 
fprintf(stderr,"libf2c : No erf \n");
return(0);
#else 
return( erf(*x) );
#endif 
}
#ifdef __cplusplus
}
#endif
