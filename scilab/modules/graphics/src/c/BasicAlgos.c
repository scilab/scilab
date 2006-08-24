/*------------------------------------------------------------------------*/
/* file: BasicAlgos.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : Several basic algorithm which can be used in several places in  */
/*        the code.                                                       */
/*------------------------------------------------------------------------*/

#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
double sciFindStPosMin( double x[], int n )
{
  double min ;
  int i ;

  if ( n <= 0 )
  {
    return -1.0 ;
  }

  min = x[0] ;

  for ( i = 1 ; i < n ; i++ )
  {
    if ( x[i] > 0.0 && x[i] < min )
    {
      min = x[i] ;
    }
  }

  /* if we have found at least one positive value in x, min strictly positive */
  return min ;
}
/*------------------------------------------------------------------------*/
int C2F(entier2d)( integer * n, double * dx,integer * s )
{
  integer ix;
  for (ix = *n -1 ; ix >= 0; --ix) { dx[ix] = (double) s[ix]; }
  return 0;
}  
/*------------------------------------------------------------------------*/
