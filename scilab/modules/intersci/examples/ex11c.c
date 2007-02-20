
#include "../../routines/machine.h"

/*******************************************
 * complex matrices 
 * are transmited as ar,ai,itra 
 *                   real part, imaginary part and a descriptor flag 
 *                   the descriptor flag is 1 for a complex matrix 0 if the 
 *                   matrix is real valued
 *******************************************/

int F2C(ext11c)( ar,ai,itra, br,bi,itrb, m,n)
     int *n,*m,*itra,*itrb;
     double *ar,*ai, *br,*bi;
{   
  int k;
  for (k = 0; k < *n*(*m); ++k) 
      {
	if ( *itra == 1) 
	  br[k] = ai[k];
	else 
	  br[k] = 0.0;
	bi[k] = ar[k];
      }
  *itrb=1;
  return(0);
}

