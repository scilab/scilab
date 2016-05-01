/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/

#include "dynlib_differential_equations.h"

// ode / odedc
typedef void(*ode_f_t)(int *n, double *t, double *y, double *ydot);
typedef void(*func_jac_t)(int *n, double *t, double *y, int *ml, int *mu, double *J, int *nrpd);
typedef void(*func_g_t)(int *n, double *t, double *y, int *ng, double *gout);

extern double C2F(lsoda)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);
extern double C2F(lsode)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);
extern double C2F(lsodar)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*, func_g_t, int*, int*);
extern double C2F(lsdisc)(ode_f_t f, int* neq, double* y, double* t, double* tout, double* rwork, int* lrw, int* istate);
extern double C2F(lsrgk)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);
extern double C2F(rkf45)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);
extern double C2F(rksimp)(ode_f_t, int*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, func_jac_t, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP void ode_f(int* n, double* t, double* y, double* ydot);
DIFFERENTIAL_EQUATIONS_IMPEXP void ode_jac(int *n, double *t, double *y, int *ml, int *mu, double *J, int *nrpd);
DIFFERENTIAL_EQUATIONS_IMPEXP void ode_g(int *n, double *t, double *y, int *ng, double *gout);

// intg
typedef double(*intg_f_t)(double*);

extern double C2F(dqags)(intg_f_t f, double* a, double* b, double* epsabs, double* epsrel, double* result, double* abserr, int* neval, int* ier, int* limit, int* lenw, int* last, int* iwork, double* dwork);

DIFFERENTIAL_EQUATIONS_IMPEXP double intg_f(double*);

// int2d
typedef double(*int2d_f_t)(double*, double*);

extern double C2F(twodq)(int2d_f_t, int*, double*, double*, double*, int*, int*, int*, double*, double*, int*, int*, int*, int*, double*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP double int2d_f(double*, double*);

// int3d
typedef void(*int3d_f_t)(double*, int*, double*);

extern double C2F(dcutet)(int3d_f_t, int*, double*, int*, int*, int*, double*, double*, int*, int*, int*, double*, double*, int*, int*, double*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP void int3d_f(double*, int*, double*);

// feval
typedef void(*feval_f_t)(int*, double*, double*, double*, int*);

// bvode
typedef void(*bvode_idd_t)(int*, double*, double*);
typedef void(*bvode_ddd_t)(double*, double*, double*);

extern double C2F(colnew)(int*, int*, double*, double*, double*, int*, int*, double*, double*, int*, double*, int*, bvode_ddd_t, bvode_ddd_t, bvode_idd_t, bvode_idd_t, bvode_ddd_t);

extern double C2F(appsln)(double*, double*, double*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP void bvode_gsub(int*, double*, double*);
DIFFERENTIAL_EQUATIONS_IMPEXP void bvode_dgsub(int*, double*, double*);
DIFFERENTIAL_EQUATIONS_IMPEXP void bvode_fsub(double*, double*, double*);
DIFFERENTIAL_EQUATIONS_IMPEXP void bvode_dfsub(double*, double*, double*);
DIFFERENTIAL_EQUATIONS_IMPEXP void bvode_guess(double*, double*, double*);

// impl
typedef void(*impl_f_t)(int*, double*, double*, double*, double*, int*);
typedef void(*impl_g_t)(int*, double*, double*, double*, double*, double*, int*);
typedef void(*impl_jac_t)(int*, double*, double*, double*, double*, double*, double*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP extern double C2F(lsodi)(impl_f_t, impl_g_t, impl_jac_t, int*, double*, double*, double*, double*, int*, double*, double*, int*, int*, int*, double*, int*, int*, int*, int*);

DIFFERENTIAL_EQUATIONS_IMPEXP void impl_f(int*, double*, double*, double*, double*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP void impl_g(int*, double*, double*, double*, double*, double*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP void impl_jac(int*, double*, double*, double*, double*, double*, double*, int*);

//dassl dasrt daskr
typedef void(*dassl_f_t)(double*, double*, double*, double*, int*, double*, int*);
typedef void(*dassl_jac_t)(double*, double*, double*, double*, double*, double*, int*);

extern void C2F(dassl)(dassl_f_t, int*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*, int*, int*, double*, int*, dassl_jac_t);

DIFFERENTIAL_EQUATIONS_IMPEXP void dassl_f(double*, double*, double*, double*, int*, double*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP void dassl_jac(double*, double*, double*, double*, double*, double*, int*);

//dasrt
typedef void(*dasrt_g_t)(int*, double*, double*, int*, double*, double*, int*);
extern void C2F(ddasrt)(dassl_f_t, int*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*, int*, int*, double*, int*, dassl_jac_t, dasrt_g_t, int*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP void dasrt_g(int*, double*, double*, int*, double*, double*, int*);

//daskr
typedef void(*daskr_psol_t)(int*, double*, double*, double*, double*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*);
typedef void(*daskr_pjac_t)(double*, int*, int*, double*, double*, double*, double*, double*, double*, double*, double*, double*, int*, int*, double*, int*);
extern void C2F(ddaskr)(dassl_f_t, int*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*, int*, int*, double*, int*, void* /*dassl_jac_t or daskr_pjac_t*/, daskr_psol_t, dasrt_g_t, int*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP void daskr_psol(int*, double*, double*, double*, double*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*);
DIFFERENTIAL_EQUATIONS_IMPEXP void daskr_pjac(double*, int*, int*, double*, double*, double*, double*, double*, double*, double*, double*, double*, int*, int*, double*, int*);

