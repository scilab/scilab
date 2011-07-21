
/* @(#)fdlibm.h 1.5 04/04/22 */
/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#ifdef _MSC_VER
    #ifdef FDLIBM_EXPORTS
        #define FDLIBM_IMPEXP __declspec(dllexport)
    #else
        #define FDLIBM_IMPEXP __declspec(dllimport)
    #endif
#else
    #define FDLIBM_IMPEXP
#endif


/* Sometimes it's necessary to define __LITTLE_ENDIAN explicitly
   but these catch some common cases. */

#if defined(i386) || defined(i486) || \
	defined(intel) || defined(x86) || defined(i86pc) || \
	defined(__alpha) || defined(__osf__) || defined(__x86_64__)
#define __LITTLE_ENDIAN
#endif

#ifdef __LITTLE_ENDIAN
#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x
#define __HIp(x) *(1+(int*)x)
#define __LOp(x) *(int*)x
#else
#define __HI(x) *(int*)&x
#define __LO(x) *(1+(int*)&x)
#define __HIp(x) *(int*)x
#define __LOp(x) *(1+(int*)x)
#endif

#ifdef __STDC__
#define	__P(p)	p
#else
#define	__P(p)	()
#endif

/*
 * ANSI/POSIX
 */

extern int signgam;

#define	MAXFLOAT	((float)3.40282346638528860e+38)

enum fdversion {fdlibm_ieee = -1, fdlibm_svid, fdlibm_xopen, fdlibm_posix};

#define _LIB_VERSION_TYPE enum fdversion
#define _LIB_VERSION _fdlib_version  

/* if global variable _LIB_VERSION is not desirable, one may 
 * change the following to be a constant by: 
 *	#define _LIB_VERSION_TYPE const enum version
 * In that case, after one initializes the value _LIB_VERSION (see
 * s_lib_version.c) during compile time, it cannot be modified
 * in the middle of a program
 */ 
extern  _LIB_VERSION_TYPE  _LIB_VERSION;

#define _IEEE_  fdlibm_ieee
#define _SVID_  fdlibm_svid
#define _XOPEN_ fdlibm_xopen
#define _POSIX_ fdlibm_posix

struct exception {
	int type;
	char *name;
	double arg1;
	double arg2;
	double retval;
};

#define	HUGE		MAXFLOAT

/* 
 * set X_TLOSS = pi*2**52, which is possibly defined in <values.h>
 * (one may replace the following line by "#include <values.h>")
 */

#define X_TLOSS		1.41484755040568800000e+16 

#define	DOMAIN		1
#define	SING		2
#define	OVERFLOW	3
#define	UNDERFLOW	4
#define	TLOSS		5
#define	PLOSS		6


/*
 * ANSI/POSIX
 */
FDLIBM_IMPEXP double acos __P((double));
FDLIBM_IMPEXP double asin __P((double));
FDLIBM_IMPEXP double atan __P((double));
FDLIBM_IMPEXP double atan2 __P((double, double));
FDLIBM_IMPEXP double cos __P((double));
FDLIBM_IMPEXP double sin __P((double));
FDLIBM_IMPEXP double tan __P((double));

FDLIBM_IMPEXP double cosh __P((double));
FDLIBM_IMPEXP double sinh __P((double));
FDLIBM_IMPEXP double tanh __P((double));

FDLIBM_IMPEXP double exp __P((double));
FDLIBM_IMPEXP double frexp __P((double, int *));
FDLIBM_IMPEXP double ldexp __P((double, int));
FDLIBM_IMPEXP double log __P((double));
FDLIBM_IMPEXP double log10 __P((double));
FDLIBM_IMPEXP double modf __P((double, double *));

FDLIBM_IMPEXP double pow __P((double, double));
FDLIBM_IMPEXP double sqrt __P((double));

FDLIBM_IMPEXP double ceil __P((double));
FDLIBM_IMPEXP double fabs __P((double));
FDLIBM_IMPEXP double floor __P((double));
FDLIBM_IMPEXP double fmod __P((double, double));

FDLIBM_IMPEXP double erf __P((double));
FDLIBM_IMPEXP double erfc __P((double));
FDLIBM_IMPEXP double gamma __P((double));
FDLIBM_IMPEXP double hypot __P((double, double));
FDLIBM_IMPEXP int isnan __P((double));
FDLIBM_IMPEXP int finite __P((double));
FDLIBM_IMPEXP double j0 __P((double));
FDLIBM_IMPEXP double j1 __P((double));
FDLIBM_IMPEXP double jn __P((int, double));
FDLIBM_IMPEXP double lgamma __P((double));
FDLIBM_IMPEXP double y0 __P((double));
FDLIBM_IMPEXP double y1 __P((double));
FDLIBM_IMPEXP double yn __P((int, double));

FDLIBM_IMPEXP double acosh __P((double));
FDLIBM_IMPEXP double asinh __P((double));
FDLIBM_IMPEXP double atanh __P((double));
FDLIBM_IMPEXP double cbrt __P((double));
FDLIBM_IMPEXP double logb __P((double));
FDLIBM_IMPEXP double nextafter __P((double, double));
FDLIBM_IMPEXP double remainder __P((double, double));
#ifdef _SCALB_INT
FDLIBM_IMPEXP double scalb __P((double, int));
#else
FDLIBM_IMPEXP double scalb __P((double, double));
#endif

FDLIBM_IMPEXP int matherr __P((struct exception *));

/*
 * IEEE Test Vector
 */
FDLIBM_IMPEXP double significand __P((double));

/*
 * Functions callable from C, intended to support IEEE arithmetic.
 */
FDLIBM_IMPEXP double copysign __P((double, double));
FDLIBM_IMPEXP int ilogb __P((double));
FDLIBM_IMPEXP double rint __P((double));
FDLIBM_IMPEXP double scalbn __P((double, int));

/*
 * BSD math library entry points
 */
FDLIBM_IMPEXP double expm1 __P((double));
FDLIBM_IMPEXP double log1p __P((double));

/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
#ifdef _REENTRANT
FDLIBM_IMPEXP double gamma_r __P((double, int *));
FDLIBM_IMPEXP double lgamma_r __P((double, int *));
#endif	/* _REENTRANT */

/* ieee style elementary functions */
FDLIBM_IMPEXP double __ieee754_sqrt __P((double));			
FDLIBM_IMPEXP double __ieee754_acos __P((double));			
FDLIBM_IMPEXP double __ieee754_acosh __P((double));			
FDLIBM_IMPEXP double __ieee754_log __P((double));			
FDLIBM_IMPEXP double __ieee754_atanh __P((double));			
FDLIBM_IMPEXP double __ieee754_asin __P((double));			
FDLIBM_IMPEXP double __ieee754_atan2 __P((double,double));			
FDLIBM_IMPEXP double __ieee754_exp __P((double));
FDLIBM_IMPEXP double __ieee754_cosh __P((double));
FDLIBM_IMPEXP double __ieee754_fmod __P((double,double));
FDLIBM_IMPEXP double __ieee754_pow __P((double,double));
FDLIBM_IMPEXP double __ieee754_lgamma_r __P((double,int *));
FDLIBM_IMPEXP double __ieee754_gamma_r __P((double,int *));
FDLIBM_IMPEXP double __ieee754_lgamma __P((double));
FDLIBM_IMPEXP double __ieee754_gamma __P((double));
FDLIBM_IMPEXP double __ieee754_log10 __P((double));
FDLIBM_IMPEXP double __ieee754_sinh __P((double));
FDLIBM_IMPEXP double __ieee754_hypot __P((double,double));
FDLIBM_IMPEXP double __ieee754_j0 __P((double));
FDLIBM_IMPEXP double __ieee754_j1 __P((double));
FDLIBM_IMPEXP double __ieee754_y0 __P((double));
FDLIBM_IMPEXP double __ieee754_y1 __P((double));
FDLIBM_IMPEXP double __ieee754_jn __P((int,double));
FDLIBM_IMPEXP double __ieee754_yn __P((int,double));
FDLIBM_IMPEXP double __ieee754_remainder __P((double,double));
FDLIBM_IMPEXP int    __ieee754_rem_pio2 __P((double,double*));
#ifdef _SCALB_INT
FDLIBM_IMPEXP double __ieee754_scalb __P((double,int));
#else
FDLIBM_IMPEXP double __ieee754_scalb __P((double,double));
#endif

/* fdlibm kernel function */
FDLIBM_IMPEXP double __kernel_standard __P((double,double,int));	
FDLIBM_IMPEXP double __kernel_sin __P((double,double,int));
FDLIBM_IMPEXP double __kernel_cos __P((double,double));
FDLIBM_IMPEXP double __kernel_tan __P((double,double,int));
FDLIBM_IMPEXP int    __kernel_rem_pio2 __P((double*,double*,int,int,int,const int*));
