/* Copyright INRIA */
/* Allan CORNET 2006 */

#ifndef SCICOS_DEF 
#define SCICOS_DEF 

#include "machine.h"

#if _LCC_ & FORDLL 
  #define IMPORT __declspec (dllimport)
#else
 #ifdef FORDLL 
   #define IMPORT extern  __declspec (dllimport)
 #else 
   #define IMPORT 
 #endif
#endif

typedef struct {
  int cosd;
} COSDEBUG_struct;

IMPORT COSDEBUG_struct C2F(cosdebug);


typedef struct {
  int counter;
} COSDEBUGCOUNTER_struct;

COSDEBUGCOUNTER_struct C2F(cosdebugcounter);

typedef struct {
  int solver;
} SOLVER_struct;

IMPORT  SOLVER_struct C2F(cmsolver);

typedef struct {
  integer iero;
} IERODE_struct;

IMPORT IERODE_struct C2F(ierode);

typedef struct {
  integer kfun;
} CURBLK_struct;

IMPORT CURBLK_struct C2F(curblk);

typedef struct {
  double scale;
}  RTFACTOR_struct;

IMPORT RTFACTOR_struct C2F(rtfactor);

typedef struct {
  integer halt;
}  COSHLT_struct;

IMPORT COSHLT_struct C2F(coshlt);

typedef struct {
  integer ptr;
}  SCSPTR_struct;

IMPORT SCSPTR_struct C2F(scsptr);

typedef struct {
	double rownr3[2], t0, tlast, toutc;
	integer lg0, lg1, lgx, iownr3[2], irfnd, itaskc, ngc, nge;
} LSR001_struct;

IMPORT LSR001_struct C2F(lsr001);

typedef struct {
	double tret, rowns[209], ccmax, el0, h__, hmin, hmxi, hu, rc, tn,uround;
	integer illin, init, lyh, lewt, lacor, lsavf, lwm, liwm, mxstep, mxhnil, 
		nhnil, ntrep, nslast, nyh, iowns[6], icf, ierpj, iersl, jcur, 
		jstart, kflag, l, meth, miter, maxord, maxcor, msbp, mxncf, n, nq,
		nst, nfe, nje, nqu;
} LS0001_struct;

IMPORT LS0001_struct C2F(ls0001);


typedef struct {
	double tsw, rowns2[20], pdnorm;
	integer insufr, insufi, ixpr, iowns2[2], jtyp, mused, mxordn, mxords;
} LSA001_struct;

IMPORT LSA001_struct C2F(lsa001);

typedef struct {
	integer idb;
} DBCOS_struct;

IMPORT DBCOS_struct C2F(dbcos);

typedef struct {
	double atol, rtol, ttol, deltat;
} COSTOL_struct;

IMPORT COSTOL_struct C2F(costol);

#endif /*SCICOS_DEF*/
