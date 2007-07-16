/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include "math_graphics.h"

/* 
 * we use spConfig.h for machine constants 
 * XXX : spConfig should be merged and unified 
 *       with other machine constant scilab code 
 */

#define spINSIDE_SPARSE
#include "../../sparse/includes/spConfig.h"


double Mini(double *vect, integer n);
double Maxi(double *vect,integer n);


double Mini(double *vect, integer n)
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

double Maxi(double *vect,integer n)
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
  rotate2Dim( from, center, cos( angle ), sin( angle ), dest ) ;
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
/* perform the rotation of point from to point dest given in int with angle in radian  */
void iRotate2D( int from[2], int center[2], double angle, int dest[2] )
{
  iRotate2Dim( from, center, cos( angle ), sin( angle ), dest ) ;
}

/*----------------------------------------------------------------------------*/
/* perform the rotation of point from to point to. */
/* the angle is directly given with its sine and cosine for speed */
void iRotate2Dim( int    from[2]   ,
                  int    center[2] ,
                  double cosAngle  ,
                  double sinAngle  ,
                  int    dest[2]    )
{
  int diff[2] ;

  /* put the center to (0,0) */
  diff[0] = from[0] - center[0] ;
  diff[1] = from[1] - center[1] ;

  /* turn and translate back */
  dest[0] = round( diff[0] * cosAngle - diff[1] * sinAngle + center[0] ) ;
  dest[1] = round( diff[0] * sinAngle + diff[1] * cosAngle + center[1] ) ;
}
/*----------------------------------------------------------------------------*/
/* perform an homethety point from to point dest. The 2 factors stand for the ration */
/* along the 2 coordinates */
void homothety2D( double from[2], double center[2], double factors[2], double dest[2] )
{
  dest[0] = center[0] + factors[0] * ( from[0] - center[0] ) ;
  dest[1] = center[1] + factors[1] * ( from[1] - center[1] ) ;
}
/*----------------------------------------------------------------------------*/
/* perform an homethety point from to point dest given in pixels. */
/* The 2 factors stand for the ration along the 2 coordinates */
void iHomothety2D( int from[2], int center[2], double factors[2], int dest[2] )
{
  dest[0] = round( center[0] + factors[0] * ( from[0] - center[0] ) ) ;
  dest[1] = round( center[1] + factors[1] * ( from[1] - center[1] ) ) ;
}
/*----------------------------------------------------------------------------*/
/* perform the translation of point from to point to with vector trans */
void translate2D( double from[2], double trans[2], double dest[2] )
{
  dest[0] = from[0] + trans[0] ;
  dest[1] = from[1] + trans[1] ;
}
/*----------------------------------------------------------------------------*/
void iTranslate2D( int from[2], int trans[2], int dest[2] )
{
  dest[0] = from[0] + trans[0] ;
  dest[1] = from[1] + trans[1] ;
}
/*----------------------------------------------------------------------------*/
void normalize2d( double vect[2] )
{
  double norm = NORM_2D(vect) ;
  vect[0] /= norm ;
  vect[1] /= norm ;
}
/*----------------------------------------------------------------------------*/
void iNormalize2d( int vect[2] )
{
  double norm = NORM_2D(vect) ;
  vect[0] = round( vect[0] / norm ) ;
  vect[1] = round( vect[1] / norm ) ;
}
/*----------------------------------------------------------------------------*/
void crossProduct( const double v1[3], const double v2[3], double res[3] )
{
  res[0] = v1[1] * v2[2] - v1[2] * v2[1] ;
  res[1] = v1[2] * v2[0] - v1[0] * v2[2] ;
  res[2] = v1[0] * v2[1] - v1[1] * v2[0] ;
}
/*----------------------------------------------------------------------------*/
void normalize3D( double vect[3] )
{
  double norm = NORM_3D(vect) ;
  vect[0] /= norm ;
  vect[1] /= norm ;
  vect[2] /= norm ;
}
/*----------------------------------------------------------------------------*/
/* check if two values can be considered equal given an accurracy */
int safeEqual( double val1, double val2, double accuracy )
{
  /* the val1 == val2 is put to avoid division by 0 */
  return ( val1 == val2 ) || ( Abs( val1 - val2 ) < accuracy * Max( Abs(val1), Abs(val2 ) ) ) ;
}
/*----------------------------------------------------------------------------*/
