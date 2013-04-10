/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "dynlib_differential_equations.h"

// ode / odedc
typedef int(*ode_f_t)(int *n, double *t, double *y, double *ydot);
typedef int(*func_jac_t)(int *n, double *t, double *y, int *ml, int *mu, double *J, int *nrpd);
typedef int(*func_g_t)(int *n, double *t, double *y, int *ng, double *gout);

extern double C2F(lsoda)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);
extern double C2F(lsode)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);
extern double C2F(lsodar)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*, func_g_t, int*, int*);
extern double C2F(lsdisc)(ode_f_t f, int* neq, double* y, double* t, double* tout, double* rwork, int* lrw, int* istate);
extern double C2F(lsrgk)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);
extern double C2F(rkf45)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);
extern double C2F(rksimp)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP int ode_f(int* n, double* t, double* y, double* ydot);
DIFFERENTIAL_EQUATIONS_IMPEXP int ode_jac(int *n, double *t, double *y, int *ml, int *mu, double *J, int *nrpd);
DIFFERENTIAL_EQUATIONS_IMPEXP int ode_g(int *n, double *t, double *y, int *ng, double *gout);

// intg
typedef double(*intg_f_t)(double*);

extern double C2F(dqags)(intg_f_t f, double* a, double* b, double* epsabs, double* epsrel, double* result, double* abserr, int* neval, int* ier, int* limit, int* lenw, int* last, int* iwork, double* dwork);

DIFFERENTIAL_EQUATIONS_IMPEXP double intg_f(double*);

// int2d
typedef double(*int2d_f_t)(double*, double*);

extern double C2F(twodq)(int2d_f_t, int*, double*, double*, double*, int*, int*, int*, double*, double*, int*, int*, int*, int*, double*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP double int2d_f(double*, double*);

// int3d
typedef double(*int3d_f_t)(double*, int*, double*);

extern double C2F(dcutet)(int3d_f_t, int*, double*, int*, int*, int*, double*, double*, int*, int*, int*, double*, double*, int*, int*, double*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP double int3d_f(double*, int*, double*);

// feval
typedef int(*feval_f_t)(int*, double*, double*, double*, int*);

// bvode
typedef int(*bvode_idd_t)(int*, double*, double*);
typedef int(*bvode_ddd_t)(double*, double*, double*);

extern double C2F(colnew)(int*, int*, double*, double*, double*, int*, int*, double*, double*, int*, double*, int*, bvode_ddd_t, bvode_ddd_t, bvode_idd_t, bvode_idd_t, bvode_ddd_t);

extern double C2F(appsln)(double*, double*, double*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP int bvode_gsub(int*, double*, double*);
DIFFERENTIAL_EQUATIONS_IMPEXP int bvode_dgsub(int*, double*, double*);
DIFFERENTIAL_EQUATIONS_IMPEXP int bvode_fsub(double*, double*, double*);
DIFFERENTIAL_EQUATIONS_IMPEXP int bvode_dfsub(double*, double*, double*);
DIFFERENTIAL_EQUATIONS_IMPEXP int bvode_guess(double*, double*, double*);

// impl
typedef int(*impl_f_t)(int*, double*, double*, double*, double*, int*);
typedef int(*impl_g_t)(int*, double*, double*, double*, double*, double*, int*);
typedef int(*impl_jac_t)(int*, double*, double*, double*, double*, double*, double*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP extern double C2F(lsodi)(impl_f_t, impl_g_t, impl_jac_t, int*, double*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP int impl_f(int*, double*, double*, double*, double*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP int impl_g(int*, double*, double*, double*, double*, double*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP int impl_jac(int*, double*, double*, double*, double*, double*, double*, int*);

//dassl dasrt daskr
typedef int(*dassl_f_t)(double*, double*, double*, double*, int*, double*, int*);
typedef int(*dassl_jac_t)(double*, double*, double*, double*, double*, double*, int*);

extern void C2F(dassl)(dassl_f_t, int*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*, int*, int*, double*, int*, dassl_jac_t);

DIFFERENTIAL_EQUATIONS_IMPEXP int dassl_f(double*, double*, double*, double*, int*, double*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP int dassl_jac(double*, double*, double*, double*, double*, double*, int*);

//dasrt
typedef int(*dasrt_g_t)(int*, double*, double*, int*, double*, double*, int*);
extern void C2F(ddasrt)(dassl_f_t, int*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*, int*, int*, double*, int*, dassl_jac_t, dasrt_g_t, int*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP int dasrt_g(int*, double*, double*, int*, double*, double*, int*);

//daskr
typedef int(*daskr_psol_t)(int*, double*, double*, double*, double*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*);
typedef int(*daskr_pjac_t)(double*, int*, int*, double*, double*, double*, double*, double*, double*, double*, double*, double*, int*, int*, double*, int*);
extern void C2F(ddaskr)(dassl_f_t, int*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*, int*, int*, double*, int*, void*/*dassl_jac_t or daskr_pjac_t*/, daskr_psol_t, dasrt_g_t, int*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP int daskr_psol(int*, double*, double*, double*, double*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP int daskr_pjac(double*, int*, int*, double*, double*, double*, double*, double*, double*, double*, double*, double*, int*, int*, double*, int*);

