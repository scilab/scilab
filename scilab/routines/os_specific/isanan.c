/* Copyright INRIA/ENPC */
#include <stdio.h>
#include <math.h>

#include "../machine.h"

#ifdef WIN32 
#ifndef __CYGWIN32__
#define _ISANAN
#include <float.h>
#endif 
#endif 


/* testing Nan returns 1 if a Nan is found and 0 elsewhere */
/* should be changed to use a libm isnan function when possible */

integer C2F(isanan)(x)
     double *x ;
{
#ifdef _ISANAN
  return(_isnan(*x)== 1);
#else
  return((!( *x <= 1.0 )) && (!( *x >= 1.0 )));
#endif
}



#ifdef TESTALONE 
int main()
{
  double x=0.0,y=1/x,z;
  z= y-y;
  if ( C2F(isanan)(&z) == 1) 
    fprintf(stdout,"z is a Nan\n");
  if ( C2F(isanan)(&x) == 1) 
    fprintf(stdout,"x is a Nan\n");
  if ( C2F(isanan)(&y) == 1) 
    fprintf(stdout,"y is a Nan\n");
}
			
#endif 

