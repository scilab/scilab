/* Copyright INRIA/ENPC */
#include <stdio.h>
#include <math.h>

#include "../machine.h"
#include "Os_specific.h"


#ifdef WIN32 
#ifndef __CYGWIN32__
#define _ISANAN
#include <float.h>
#endif 
#endif 


/* testing Nan returns 1 if a Nan is found and 0 elsewhere */
/* should be changed to use a libm isnan function when possible */

integer C2F(isanan)(double *x)
{
#ifdef _ISANAN
  return(_isnan(*x)== 1);
#else
  return((!( *x <= 1.0 )) && (!( *x >= 1.0 )));
#endif
}

