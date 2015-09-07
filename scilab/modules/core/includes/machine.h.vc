/** machine.h version for vc++ **/
/* Copyright INRIA/ENPC */

#ifndef MACHINE_H
#define MACHINE_H

#include <math.h>

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

/* Define if have termcap library */
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
/* #define exp10(x) pow( (double)10.0, (x) ) */
/* much faster than using pow */
#define log_10_ 2.3025850929940456840179914546844 /* log(10) */
#define exp10(x) exp( (log_10_) * (x) )
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

#define SHARED_LIB_EXT ".dll"
#define SHARED_LIB_EXTW L".dll"

/* PATH_SEPARATOR : Under Windows by default is ; */
#define PATH_SEPARATOR ";"

/* DIR_SEPARATOR : Under Windows by default is \ */
#define DIR_SEPARATOR "\\"
#define DIR_SEPARATORW L"\\"

#define HAVE_LIBINTL_H

#if defined( __INTEL_COMPILER) && !defined(_WIN64)
#define va_copy(x,y) (x=(y))
#endif

#define SIZEOF_INT 4
#define SIZEOF_LONG 4

#define bsiz 4096

#endif /* MACHINE_H  */
