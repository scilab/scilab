/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __BASE_MATH_H__
#define __BASE_MATH_H__
#include <limits.h>
#include <math.h>

#ifdef __STDC__
#include <stdlib.h>
#endif

#ifndef _MSC_VER 
#endif

#ifdef _MSC_VER 
		#include <float.h>
		#define finite(x) _finite(x) 
#endif /* _MSC_VER */

#ifdef _MSC_VER 
	#include <float.h>
	#define ISNAN(x) _isnan(x)
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

#define PI0 (int *) 0
#define PD0 (double *) 0
#define SMDOUBLE 1.e-200 /* Smalest number to avoid dividing by zero */

/* angle conversion */
#define PI_OVER_180  0.01745329251994329576913914624236578987393
#define _180_OVER_PI 57.29577951308232087665461840231273527024
#define DEG2RAD(x) ((x) * PI_OVER_180  )
#define RAD2DEG(x) ((x) * _180_OVER_PI )

#ifdef round
#undef round
#endif

#define		round(a)	(int)(((a)<0.0)?(a)-.5:(a)+.5)
#define EPSILON 1.0e-13


/** 
  if we suppose that the x transmited is in the range of integers 
  we could also use :
  #define inint(x) (( x > 0 ) ? ((int) (x + 0.5)) : ((int) (x - 0.5));
 **/

#define linint(x) ((int)  floor(x + 0.5 )) 
#define inint(x) ((int) floor(x + 0.5 ))  

#if (defined(sun) && defined(SYSV)) 
#include <ieeefp.h>
#endif

#if defined(_MSC_VER)
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

#ifndef HAVE_EXP10
#define log_10_ 2.3025850929940456840179914546844
/* Provide a macro to do exp10 */
#define exp10(x) exp( (log_10_) * (x) )
#endif

#endif /* __BASE_MATH_H__ */

