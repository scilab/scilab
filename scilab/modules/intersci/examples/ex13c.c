
#include "../../routines/machine.h"

/*******************************************
 * simple example 
 *******************************************/

int F2C(ext13c)( ar, m,n)
     int *n,*m;
     double *ar;
{   
  int k;
  for (k = 0; k < *n*(*m); ++k) 
      ar[k]= 2*ar[k];
  return(0);
}

