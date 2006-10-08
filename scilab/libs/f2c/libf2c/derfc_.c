#include "f2c.h"
#ifdef _MSC_VER
#include <stdio.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
extern double erfc();

double derfc_(x) doublereal *x;
#else
extern double erfc(double);

double derfc_(doublereal *x)
#endif
{
#ifdef _MSC_VER
	fprintf(stderr,"libf2c: No erf\n");
	return (0);
#else
return( erfc(*x) );
#endif
}
#ifdef __cplusplus
}
#endif
