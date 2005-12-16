/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#ifndef __SCIMATH_H__
#define __SCIMATH_H__
#include "../machine.h"


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

#include "Graphics.h" 

#endif /* SCI_MATH.HG */

