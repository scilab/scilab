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

/*----------------------------------------------------------------------------*/

/* perform the rotation of point from to point dest  */
void rotate2D( double from[2], double center[2], double angle, double dest[2] )
{
  double cosAngle = cos( angle ) ;
  double sinAngle = sin( angle ) ;
  rotate2Dim( from, center, cosAngle, sinAngle, dest ) ;
}

/*----------------------------------------------------------------------------*/
/* perform the rotation of point from to point to. */
/* the angle is directly given with its sine and cosine for speed */
void rotate2Dim( double from[2]   ,
                 double center[2] ,
                 double cosAngle  ,
                 double sinAngle  ,
                 double dest[2]    )
{
  double diff[2] ;

  /* put the center to (0,0) */
  diff[0] = from[0] - center[0] ;
  diff[1] = from[1] - center[1] ;

  /* turn and translate back */
  dest[0] = diff[0] * cosAngle - diff[1] * sinAngle + center[0] ;
  dest[1] = diff[0] * sinAngle + diff[1] * cosAngle + center[1] ;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

/* perform the translation of point from to point to with vector trans */
void translate2D( double from[2], double trans[2], double dest[2] )
{
  dest[0] = from[0] + trans[0] ;
  dest[1] = from[1] + trans[1] ;
}

/*----------------------------------------------------------------------------*/

/* check if two values can be considered equal given an accurracy */
int safeEqual( double val1, double val2, double accuracy )
{
  /* the val1 == val2 is put to avoid division by 0 */
  return ( val1 == val2 ) || ( Abs( val1 - val2 ) < accuracy * Max( Abs(val1), Abs(val2 ) ) ) ;
}

/*----------------------------------------------------------------------------*/
