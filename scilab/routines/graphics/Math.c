/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include "Math.h"

/* 
 * we use spConfig.h for machine constants 
 * XXX : spConfig should be merged and unified 
 *       with other machine constant scilab code 
 */

#define spINSIDE_SPARSE
#if defined(THINK_C) || defined (__MWERKS__)
#include "::sparse:spConfig.h" 
#else
#include "../sparse/spConfig.h"
#endif

double Mini(vect, n)
     double *vect;
     integer n;
{
  int i;
  double vmin;
  vmin = LARGEST_REAL;
  for (i = 0 ; i < n ; i++)
    /*    if ( isinf(vect[i])== 0 && isnan(vect[i])==0 && vect[i] < vmin)  */
    if ( finite(vect[i])== 1 && vect[i] < vmin) 
      vmin=vect[i];
  return(vmin);
}

double Maxi(vect, n)
     double *vect;
     integer n;
{
  int i;
  double maxi;
  maxi= - LARGEST_REAL;
  for (i =0 ; i < n ; i++)
    /* if ( isinf(vect[i])== 0 && isnan(vect[i])==0 && vect[i] > maxi) */
    if ( finite(vect[i])== 1 && vect[i] > maxi) 
      maxi=vect[i];
  return(maxi);
}
