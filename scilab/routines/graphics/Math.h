/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#ifndef __SCIMATH_H__
#define __SCIMATH_H__
#include "../machine.h"
#include <limits.h>

#include <math.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "../os_specific/sci_mem_alloc.h"  /* malloc */


#ifdef WIN32 
#if !(defined __CYGWIN32__)
#include <float.h>
#define finite(x) _finite(x) 
#endif 
#else  /* WIN32 */ 
/** This should only be provided when finite prototype is missing **/
/** XXX : to be tested **/
#ifndef __cplusplus
int finite __PARAMS((double));
#endif
#endif /* WIN32 */

#ifdef WIN32 
#if !(defined __CYGWIN32__)
#include <float.h>
#define ISNAN(x) _isnan(x)
#else 
#define ISNAN(x) isnan(x)
#endif /* __CYGWIN32__ */
#else 
#define ISNAN(x) isnan(x)
#endif 

#define Abs(x) ( ( (x) >= 0) ? (x) : -( x) )
#ifndef Min
#define Min(x,y)	(((x)<(y))?(x):(y))
#endif 

#ifndef Max 
#define Max(x,y)	(((x)>(y))?(x):(y))
#endif

#define PI0 (integer *) 0
#define PD0 (double *) 0
#define SMDOUBLE 1.e-200 /* Smalest number to avoid dividing by zero */

/* angle conversion */
#define PI_OVER_180  0.01745329251994329576913914624236578987393
#define _180_OVER_PI 57.29577951308232087665461840231273527024
#define DEG2RAD(x) ((x) * PI_OVER_180  )
#define RAD2DEG(x) ((x) * _180_OVER_PI )

#define		round(a)	(int)(((a)<0.0)?(a)-.5:(a)+.5)
#define EPSILON 1.0e-13



/** 
  #define linint(x) ((integer) floor(x)) 
  #define inint(x) ((int) floor(x))  
**/

/** 
  if we suppose that the x transmited is in the range of integers 
  we could also use :
  #define inint(x) (( x > 0 ) ? ((int) (x + 0.5)) : ((int) (x - 0.5));
 **/

#define linint(x) ((integer) floor(x + 0.5 )) 
#define inint(x) ((int) floor(x + 0.5 ))  

/* XWindow function arguments can be  int16 or unsigned16 */

#define SCIINT16   short
#define INT16MAX   SHRT_MAX
#define INT16MIN   SHRT_MIN
#define int16max   0x7FFF
#define uns16max   0xFFFF

#if (defined(sun) && defined(SYSV)) 
#include <ieeefp.h>
#endif

#if defined(THINK_C)|| defined(WIN32)
  #define M_PI 3.14159265358979323846
#else
  #if defined(HAVE_VALUES_H)
    #include <values.h>
  #else
    #if defined(HAVE_LIMITS_H)
     #include <limits.h>
    #endif
  #endif
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846 
#endif


extern double exp10 __PARAMS((double));

/* prototypes */

/*----------------------------------------------------------------------------*/
/* perform the rotation of point from to point dest with angle in radian  */
void rotate2D( double from[2], double center[2], double angle, double dest[2] ) ;

/* perform the rotation of point from to point to. */
/* the angle is directly given with its sine and cosine for speed */
void rotate2Dim( double from[2]   ,
                 double center[2] ,
                 double cosAngle  ,
                 double sinAngle  ,
                 double dest[2]    ) ;

/* perform the translation of point from to point dest with vector trans */
void translate2D( double from[2], double trans[2], double dest[2] ) ;
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check if two values can be considered equal given an accurracy */
int safeEqual( double val1, double val2, double accuracy ) ;

/* with a macro it is faster */
/* the x == y test is put to avoid division by 0 */
#define SAFE_EQUAL(x,y,acc) ( (x) == (y) ) || ( Abs( (x) - (y) ) < (acc) * Max( Abs( (x) ), Abs( (y) ) ) )
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* truncate an integer to a 16 bits integer. Overflow value are trucated to the min */
/* and max of 16 bits integer. */
#define INT_2_16B(i) ( (i) > INT16MAX ? (INT16MAX) : ( (i) < INT16MIN ? INT16MIN : ((SCIINT16) i)) )
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* truncate a float or double to an int. Overflow value are trucated to the min */
/* and max of 32 bits integer. */
#define FLOAT_2_INT(f) ( (f) > INT_MAX ? (INT_MAX) : ( (f) < INT_MIN ? (INT_MIN) : ( inint( f ) ) ) )
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return the number of digits of a positive integer (ie for 10236 it is 5 )  */
/* the added 0.5 is to avoid trouble with 10^i which could become 9.999999999 */
#define GET_NB_DIGITS(i) ( (i) == 0 ? 1 : ((int) floor( log10( (i) + 0.5 ) ) ) + 1 )
/*----------------------------------------------------------------------------*/



#include "Graphics.h" 

#endif /* SCI_MATH.HG */

