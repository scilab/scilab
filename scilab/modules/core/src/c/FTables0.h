#ifndef __FTABLES0__
#define __FTABLES0__

#include "machine.h"

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

#define ARGS_ffeval integer*,double *,double *,double *,integer*,char *
typedef void (*ffevalf)(ARGS_ffeval);


/***********************************
 * ode   (fydot and fjac ) 
 ***********************************/

#define ARGS_fydot integer*,double *,double *,double *
typedef void (*fydotf)(ARGS_fydot);

#define ARGS_fjac integer*,double *,double *,integer*,integer*,double*,integer*
typedef void (*fjacf)(ARGS_fjac);


/***********************************
 * impl   (  fres, fadda, fj2 )
 ***********************************/

#define ARGS_fres integer*,double *,double *,double *,double*,integer*
typedef void (*fresf)(ARGS_fres);


#define ARGS_fadda integer*,double *,double *,integer*,integer*,double*,integer*
typedef void (*faddaf)(ARGS_fadda);

#define ARGS_fj2 integer *,double *,double *,double *,integer *,integer *,double*,integer *
typedef void (*fj2f)(ARGS_fj2);

/***********************************
 * corr ( dgetx dgety )
 ***********************************/

#define ARGS_dgetx double *,integer*,integer*
typedef void (*dgetxf)(ARGS_dgetx);

#define ARGS_dgety double *,integer*,integer*
typedef void (*dgetyf)(ARGS_dgety);


/***********************************
 * Search Table for colnew 
 *   corr uses : fcoldg , fcolg, fcoldf,fcolf,fcolgu
 ***********************************/

#define ARGS_fcoldg integer*,double *,double*
typedef void (*fcoldgf)(ARGS_fcoldg);


#define ARGS_fcolg integer*,double *,double*
typedef void (*fcolgf)(ARGS_fcolg);


#define ARGS_fcoldf double *,double *,double*
typedef void (*fcoldff)(ARGS_fcoldf);

#define ARGS_fcolf double *,double *,double*
typedef void (*fcolff)(ARGS_fcolf);


#define ARGS_fcolgu double *,double *,double*
typedef void (*fcolguf)(ARGS_fcolgu);


/***********************************
 * Search Table for intg 
 ***********************************/

#define ARGS_fintg double *
typedef double * (*fintgf)(ARGS_fintg);

/***********************************
 * Search Table for int2d
 ***********************************/

#define ARGS_fint2d double *,double *
typedef double * (*fint2df)(ARGS_fint2d);

/***********************************
 * Search Table for int3d
 ***********************************/

#define ARGS_fint3d double *,integer*,double *
typedef void (*fint3df)(ARGS_fint3d);

/***********************************
 * Search Table for fsolve 
 ***********************************/

#define ARGS_fsolvf integer*,double *,double*,integer*
typedef void (*fsolvff)(ARGS_fsolvf);

#define ARGS_fsolvj integer*,double*,double*,integer*
typedef void (*fsolvjf)(ARGS_fsolvj);

/***********************************
 * Search Table for lsqrsolve 
 ***********************************/

#define ARGS_lsqrsolvf integer*,integer*,double *,double*,integer*
typedef void (*lsqrsolvff)(ARGS_lsqrsolvf);

#define ARGS_lsqrsolvj integer*,integer*,double*,double*,integer*,integer*
typedef void (*lsqrsolvjf)(ARGS_lsqrsolvj);


/***********************************
 * Search Table for foptim 
 ***********************************/

#define ARGS_foptim integer*,integer*,double *,double*,double*,integer*,float*,double*
typedef void (*foptimf)(ARGS_foptim);

/***********************************
 * Search Table for fschur
 ***********************************/

#define ARGS_fschur integer*,double *,double*,double*,double*
typedef integer * (*fschurf)(ARGS_fschur);

/***********************************
 * Search Table for schsel
 ***********************************/

#define ARGS_schsel double *,double*
typedef integer * (*schself)(ARGS_schsel);

/***********************************
 * Search Table for zchsel
 ***********************************/

#define ARGS_zchsel doublecmplx *
typedef integer * (*zchself)(ARGS_zchsel);

/***********************************
 * Search Table for gshsel
 ***********************************/

#define ARGS_gshsel double *,double*,double*
typedef integer * (*gshself)(ARGS_gshsel);

/***********************************
 * Search Table for gzhsel
 ***********************************/

#define ARGS_gzhsel doublecmplx *,doublecmplx *
typedef integer * (*gzhself)(ARGS_gzhsel);

/***********************************
 * Search Table for odedc
 ***********************************/

#define ARGS_fydot2 integer*, integer*,integer*,double *,double*,double* 
#define ARGS_fydot2f integer *, double *, double *, double *
typedef integer * (*fydot2f)(ARGS_fydot2);


/***********************************
 * Search Table for dassl 
 ***********************************/

#define ARGS_fresd double *,double*,double*,double*,integer*,double*,integer*
typedef integer * (*fresdf)(ARGS_fresd);

#define ARGS_fjacd double *,double*,double*,double*,double*,double*,integer*
typedef integer * (*fjacdf)(ARGS_fjacd);

/***********************************
 * Search Table for dasrt 
 ***********************************/

#define ARGS_fsurfd integer*,double *,double*,integer*,double*,double*,integer*
typedef integer * (*fsurfdf)(ARGS_fsurfd);


#define ARGS_fsurf integer*,double *,double*,integer*,double*
typedef integer * (*fsurff)(ARGS_fsurf);


/***********************************
 * Search Table for fbutn
 ***********************************/


#define ARGS_fbutn char *,integer*,integer*
typedef integer * (*fbutnf)(ARGS_fbutn);



/***********************************
 * Search Table for interf
 ***********************************/

#define ARGS_interf void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *
typedef integer * (*interff)(ARGS_interf);


#define ARGS_interf1 char *,long int
typedef integer * (*interff1)(ARGS_interf1);

#endif /*__FTABLES0__*/
