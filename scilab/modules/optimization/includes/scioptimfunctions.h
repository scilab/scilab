/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
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

#include "dynlib_optimization.h"

// optim
typedef void (*costf_t)(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);
typedef void (*fuclid_t)(int *n, double *x, double *y, double *ps, int *ti, float *tr, double *td);
typedef void (*ctonb_t)(int *n, double *u, double *v, int *ti, float *tr, double *td);
typedef void (*ctcab_t)(int *n, double *u, double *v, int *ti, float *tr, double *td);
extern void C2F(n1qn1)(costf_t, int*, double*, double*, double*, double*, double*, int*, int*, int*, int*, int*, double*, int*, float*, double*);
extern void C2F(n1qn3)(costf_t, fuclid_t, ctonb_t, ctcab_t, int*, double*, double*, double*, double*, double*, double*, int*, int*, int*, int*, int*, double*, int*, int*, float*, double*);
extern void C2F(n1fc1)(costf_t, fuclid_t, int*, double*, double*, double*, double*, double*, double*, double*, int*, int*, int*, int*, int*, int*, int*, double*, double*, int*, float*, double*);
extern void C2F(qnbd)(int*, costf_t, int*, double*, double*, double*, int*, int*, double*, int*, int*, double*, double*, double*, double*, double*, double*, int*, double*, int*, int*, int*, int*, float*, double*);
extern void C2F(gcbd)(int*, costf_t, char*, int*, double*, double*, double*, int*, int*, double*, int*, int*, double*, double*, double*, double*, double*, double*, int*, double*, int*, int*, int*, int*, float*, double*);
OPTIMIZATION_IMPEXP void costf(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td);
OPTIMIZATION_IMPEXP void C2F(fuclid)(int *n, double *x, double *y, double *ps, int *ti, float *tr, double *td);
OPTIMIZATION_IMPEXP void C2F(ctonb)(int *n, double *u, double *v, int *ti, float *tr, double *td);
OPTIMIZATION_IMPEXP void C2F(ctcab)(int *n, double *u, double *v, int *ti, float *tr, double *td);

// fsolve
typedef void (*fct_t)(int *n, double *x, double *v, int *iflag);
typedef void (*jac_t)(int *n, double *x, double *v, double *jac, int *ldjac, int *iflag);
extern void C2F(hybrd1)(fct_t, int* n, double* x, double* v, double* tol, int* info, double* pdblWork, int* iWorkSize);
extern void C2F(hybrj1)(jac_t, int* n, double* x, double* v, double* jac, int* ldjac, double* tol, int* info, double* pdblWork, int* iWorkSize);
OPTIMIZATION_IMPEXP void fct(int *n, double *x, double *v, int *iflag);
OPTIMIZATION_IMPEXP void jac(int *n, double *x, double *v, double *jac, int *ldjac, int *iflag);

// lsqrsolve
typedef void (*lsqrfct_t)(int *m, int *n, double *x, double *v, int *iflag);
typedef void (*lsqrjac_t)(int *m, int *n, double *x, double *v, double *jac, int *ldjac, int *iflag);
typedef void (*lsqrjac_ext_t)(int *m, int *n, double *x, double *jac, int *ldjac, int *iflag);
extern void C2F(lmder)( lsqrjac_t, int*, int*, double*, double*, double*, int*, double*, double*, double*, int*, double*, int*, double*, int*, int*, int*, int*, int*, double*, double*, double*, double*, double*);
extern void C2F(lmdif)( lsqrfct_t, int*, int*, double*, double*, double*, double*, double*, int*, double*, double*, int*, double*, int*, int*, int*, double*, int*, int*, double*, double*, double*, double*, double*);
OPTIMIZATION_IMPEXP void lsqrfct(int *m, int *n, double *x, double *v, int *iflag);
OPTIMIZATION_IMPEXP void lsqrjac(int *m, int *n, double *x, double *v, double *jac, int *ldjac, int *iflag);
