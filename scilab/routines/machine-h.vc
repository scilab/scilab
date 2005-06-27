/** machine.h version for vc++ **/
/* Copyright INRIA/ENPC */

#ifndef MACHINE_H 
#define MACHINE_H 

/* Define for using dld for sunos */
/* #undef SUNOSDLD */

/* Define if leading underscores */
/* #undef WLU */

/* Define if trailing underscores */
#define WTU 1

/* Define if use sharpsigns */
#define USE_SHARP_SIGN 1

/* Define if have exp10 */
/* #undef HAVE_EXP10 */

/* Define if have getwd */
#define HAVE_GETWD 1

/* Define if have strerror: used in fileio/misc.c */
#define HAVE_STRERROR 1

/* Define if have termcap library: used in os_specific/zzledt.c */
#define HAVE_TERMCAP 1

/* CNAME(x,y) ==> xy **/

#if defined(USE_SHARP_SIGN)
#define CNAME(name1,name2) name1##name2
#else 
#define CNAME(name1,name2) name1/**/name2
#endif

/* Define  C2F and F2C entry point conversion */
#if defined(WTU)
#if defined(USE_SHARP_SIGN)
#define C2F(name) name##_
#define F2C(name) name##_
#else 
#define C2F(name) name/**/_
#define F2C(name) name/**/_
#endif
#else
#define C2F(name) name
#define F2C(name) name
#endif

/* Define some functions */

#if !defined(HAVE_EXP10)
#define exp10(x) pow((double) 10.0,x)
#endif

#if !defined(HAVE_GETWD)
#define getwd(x) getcwd(x,1024) /* you must define char x[1024] */
#endif

/* 
   Define integer C type which must fit Fortran integer
   For Scilab to work, the rule is: 
          size of Fortran double precision = 2 * size of Fortran integer

   At the present time, we suppose:
       size of Fortran integer = 4 bytes
       size of Fortran double precision = 8 bytes
       size of C int = 4 bytes
*/

typedef int integer;



/*  params.h */
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		paramlist
#endif

#ifndef  _PARAMS
#define  _PARAMS(paramlist)		paramlist
#endif


/** Intel Blas library on win32 */

#ifdef MKL 
#include "MKL.h" 
#endif 




#endif /* MACHINE_H  */
