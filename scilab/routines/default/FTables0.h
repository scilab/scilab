#ifndef __FTABLES0__
#define __FTABLES0__

#include "../machine.h"

#define OK 1
#define FAIL 0

typedef void (*voidf)();

typedef struct { double r, i; } doublecmplx;

typedef struct {
  char *name;
  voidf f;
} FTAB;

#define MAXNAME 32
static char buf[MAXNAME];

/***********************************
 * feval (ffeval)
 ***********************************/

#if defined(__STDC__)
#define ARGS_ffeval integer*,double *,double *,double *,integer*,char *
typedef void (*ffevalf)(ARGS_ffeval);
#else
#define ARGS_ffeval /**/
typedef void (*ffevalf)();
#endif 

/***********************************
 * ode   (fydot and fjac ) 
 ***********************************/

#if defined(__STDC__)
#define ARGS_fydot integer*,double *,double *,double *
typedef void (*fydotf)(ARGS_fydot);
#else
#define ARGS_fydot /**/
typedef void (*fydotf)();
#endif 

#if defined(__STDC__)
#define ARGS_fjac integer*,double *,double *,integer*,integer*,double*,integer*
typedef void (*fjacf)(ARGS_fjac);
#else
#define ARGS_fjac /**/
typedef void (*fjacf)();
#endif 


/***********************************
 * impl   (  fres, fadda, fj2 )
 ***********************************/

#if defined(__STDC__)
#define ARGS_fres integer*,double *,double *,double *,double*,integer*
typedef void (*fresf)(ARGS_fres);
#else
#define ARGS_fres /**/
typedef void (*fresf)();
#endif 


#if defined(__STDC__)
#define ARGS_fadda integer*,double *,double *,integer*,integer*,double*,integer*
typedef void (*faddaf)(ARGS_fadda);
#else
#define ARGS_fadda /**/
typedef void (*faddaf)();
#endif 

#if defined(__STDC__)
#define ARGS_fj2 integer *,double *,double *,double *,integer *,integer *,double*,integer *
typedef void (*fj2f)(ARGS_fj2);
#else
#define ARGS_fj2 /**/
typedef void (*fj2f)();
#endif 

/***********************************
 * corr ( dgetx dgety )
 ***********************************/

#if defined(__STDC__)
#define ARGS_dgetx double *,integer*,integer*
typedef void (*dgetxf)(ARGS_dgetx);
#else
#define ARGS_dgetx /**/
typedef void (*dgetxf)();
#endif 


#if defined(__STDC__)
#define ARGS_dgety double *,integer*,integer*
typedef void (*dgetyf)(ARGS_dgety);
#else
#define ARGS_dgety /**/
typedef void (*dgetyf)();
#endif 

/***********************************
 * Search Table for colnew 
 *   corr uses : fcoldg , fcolg, fcoldf,fcolf,fcolgu
 ***********************************/


#if defined(__STDC__)
#define ARGS_fcoldg integer*,double *,double*
typedef void (*fcoldgf)(ARGS_fcoldg);
#else
#define ARGS_fcoldg /**/
typedef void (*fcoldgf)();
#endif 

#if defined(__STDC__)
#define ARGS_fcolg integer*,double *,double*
typedef void (*fcolgf)(ARGS_fcolg);
#else
#define ARGS_fcolg /**/
typedef void (*fcolgf)();
#endif 

#if defined(__STDC__)
#define ARGS_fcoldf double *,double *,double*
typedef void (*fcoldff)(ARGS_fcoldf);
#else
#define ARGS_fcoldf /**/
typedef void (*fcoldff)();
#endif 


#if defined(__STDC__)
#define ARGS_fcolf double *,double *,double*
typedef void (*fcolff)(ARGS_fcolf);
#else
#define ARGS_fcolf /**/
typedef void (*fcolff)();
#endif 


#if defined(__STDC__)
#define ARGS_fcolgu double *,double *,double*
typedef void (*fcolguf)(ARGS_fcolgu);
#else
#define ARGS_fcolgu /**/
typedef void (*fcolguf)();
#endif 


/***********************************
 * Search Table for intg 
 ***********************************/


#if defined(__STDC__)
#define ARGS_fintg double *
typedef double * (*fintgf)(ARGS_fintg);
#else
#define ARGS_fintg /**/
typedef double * (*fintgf)();
#endif 

/***********************************
 * Search Table for int2d
 ***********************************/


#if defined(__STDC__)
#define ARGS_fint2d double *,double *
typedef double * (*fint2df)(ARGS_fint2d);
#else
#define ARGS_fint2d /**/
typedef double * (*fint2df)();
#endif 

/***********************************
 * Search Table for int3d
 ***********************************/


#if defined(__STDC__)
#define ARGS_fint3d double *,integer*,double *
typedef void (*fint3df)(ARGS_fint3d);
#else
#define ARGS_fint3d /**/
typedef void (*fint3df)();
#endif 


/***********************************
 * Search Table for fsolve 
 ***********************************/

#if defined(__STDC__)
#define ARGS_fsolvf integer*,double *,double*,integer*
typedef void (*fsolvff)(ARGS_fsolvf);
#else
#define ARGS_fsolvf /**/
typedef void (*fsolvff)();
#endif 

#if defined(__STDC__)
#define ARGS_fsolvj integer*,double*,double*,integer*
typedef void (*fsolvjf)(ARGS_fsolvj);
#else
#define ARGS_fsolvj /**/
typedef void (*fsolvjf)();
#endif 

/***********************************
 * Search Table for lsqrsolve 
 ***********************************/

#if defined(__STDC__)
#define ARGS_lsqrsolvf integer*,integer*,double *,double*,integer*
typedef void (*lsqrsolvff)(ARGS_lsqrsolvf);
#else
#define ARGS_lsqrsolvf /**/
typedef void (*lsqrsolvff)();
#endif 

#if defined(__STDC__)
#define ARGS_lsqrsolvj integer*,integer*,double*,double*,integer*,integer*
typedef void (*lsqrsolvjf)(ARGS_lsqrsolvj);
#else
#define ARGS_lsqrsolvj /**/
typedef void (*lsqrsolvjf)();
#endif 


/***********************************
 * Search Table for foptim 
 ***********************************/

#if defined(__STDC__)
#define ARGS_foptim integer*,integer*,double *,double*,double*,integer*,float*,double*
typedef void (*foptimf)(ARGS_foptim);
#else
#define ARGS_foptim /**/
typedef void (*foptimf)();
#endif 


/***********************************
 * Search Table for fschur
 ***********************************/

#if defined(__STDC__)
#define ARGS_fschur integer*,double *,double*,double*,double*
typedef integer * (*fschurf)(ARGS_fschur);
#else
#define ARGS_fschur /**/
typedef integer * (*fschurf)();
#endif 

/***********************************
 * Search Table for schsel
 ***********************************/

#if defined(__STDC__)
#define ARGS_schsel double *,double*
typedef integer * (*schself)(ARGS_schsel);
#else
#define ARGS_schsel /**/
typedef integer * (*schself)();
#endif 

/***********************************
 * Search Table for zchsel
 ***********************************/

#if defined(__STDC__)
#define ARGS_zchsel doublecmplx *
typedef integer * (*zchself)(ARGS_zchsel);
#else
#define ARGS_zchsel /**/
typedef integer * (*zchself)();
#endif 

/***********************************
 * Search Table for gshsel
 ***********************************/

#if defined(__STDC__)
#define ARGS_gshsel double *,double*,double*
typedef integer * (*gshself)(ARGS_gshsel);
#else
#define ARGS_gshsel /**/
typedef integer * (*gshself)();
#endif 

/***********************************
 * Search Table for gzhsel
 ***********************************/

#if defined(__STDC__)
#define ARGS_gzhsel doublecmplx *,doublecmplx *
typedef integer * (*gzhself)(ARGS_gzhsel);
#else
#define ARGS_gzhsel /**/
typedef integer * (*gzhself)();
#endif 

/***********************************
 * Search Table for odedc
 ***********************************/

#if defined(__STDC__)
#define ARGS_fydot2 integer*, integer*,integer*,double *,double*,double* 
typedef integer * (*fydot2f)(ARGS_fydot2);
#else
#define ARGS_fydot2 /**/
typedef integer * (*fydot2f)();
#endif 


/***********************************
 * Search Table for dassl 
 ***********************************/

#if defined(__STDC__)
#define ARGS_fresd double *,double*,double*,double*,integer*,double*,integer*
typedef integer * (*fresdf)(ARGS_fresd);
#else
#define ARGS_fresd /**/
typedef integer * (*fresdf)();
#endif 

#if defined(__STDC__)
#define ARGS_fjacd double *,double*,double*,double*,double*,double*,integer*
typedef integer * (*fjacdf)(ARGS_fjacd);
#else
#define ARGS_fjacd /**/
typedef integer * (*fjacdf)();
#endif 



/***********************************
 * Search Table for dasrt 
 ***********************************/

#if defined(__STDC__)
#define ARGS_fsurfd integer*,double *,double*,integer*,double*,double*,integer*
typedef integer * (*fsurfdf)(ARGS_fsurfd);
#else
#define ARGS_fsurfd /**/
typedef integer * (*fsurfdf)();
#endif 

#if defined(__STDC__)
#define ARGS_fsurf integer*,double *,double*,integer*,double*
typedef integer * (*fsurff)(ARGS_fsurf);
#else
#define ARGS_fsurf /**/
typedef integer * (*fsurff)();
#endif 


/***********************************
 * Search Table for fbutn
 ***********************************/

#if defined(__STDC__)
#define ARGS_fbutn char *,integer*,integer*
typedef integer * (*fbutnf)(ARGS_fbutn);
#else
#define ARGS_fbutn /**/
typedef integer * (*fbutnf)();
#endif 



/***********************************
 * Search Table for interf
 ***********************************/

#if defined(__STDC__)
#define ARGS_interf void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *
typedef integer * (*interff)(ARGS_interf);
#else
#define ARGS_interf /**/
typedef integer * (*interff)();
#endif 

#if defined(__STDC__)
#define ARGS_interf1 char *,long int
typedef integer * (*interff1)(ARGS_interf1);
#else
#define ARGS_interf1 /**/
typedef integer * (*interff1)();
#endif 

#endif /*__FTABLES0__*/
