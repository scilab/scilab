/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

#ifndef __SCIMATH_H__
#define __SCIMATH_H__
#include "../machine.h"

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#ifdef WIN32 
#if !(defined __CYGWIN32__) && !(defined __ABSC__)
#include <float.h>
#define finite(x) _finite(x) 
#endif 
#else  /* WIN32 */ 
/** This should only be provided when finite prototype is missing **/
/** XXX : to be tested **/
int finite __PARAMS((double));
#endif /* WIN32 */

#ifdef WIN32 
#if !(defined __CYGWIN32__) && !(defined __ABSC__)
#include <float.h>
#define ISNAN(x) _isnan(x)
#else 
#define ISNAN(x) isnan(x)
#endif /* __CYGWIN32__ */
#else 
#define ISNAN(x) isnan(x)
#endif 

#define Abs(x) ( ( (x) >= 0) ? (x) : -( x) )
#define Min(x,y)	(((x)<(y))?(x):(y))
#define Max(x,y)	(((x)>(y))?(x):(y))

#define PI0 (integer *) 0
#define PD0 (double *) 0
#define SMDOUBLE 1.e-200 /* Smalest number to avoid dividing by zero */

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

#ifdef lint5
#include <sys/stdtypes.h>
#define MALLOC(x) malloc(((size_t) x))
#define FREE(x) if (x  != NULL) free((void *) x);
#define REALLOC(x,y) realloc((void *) x,(size_t) y)
#else
#define MALLOC(x) malloc(((unsigned) x))
#define FREE(x) if (x  != NULL) free((char *) x);
#define REALLOC(x,y) realloc((char *) x,(unsigned) y)
#endif

#if (defined(sun) && defined(SYSV)) 
#include <ieeefp.h>
#endif

#if defined(THINK_C)|| defined(WIN32)
#define M_PI	3.14159265358979323846
#else
#if defined(HAVE_VALUES_H)
#include <values.h>
#else
#ifndef M_PI
#define M_PI    3.14159265358979323846 
#endif
#endif
#endif


extern double exp10 __PARAMS((double));

/* prototypes */

#include "Graphics.h" 

#endif /* SCI_MATH.HG */

