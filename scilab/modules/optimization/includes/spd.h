/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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
#ifndef __SPD_H__
#define __SPD_H__

#include "machine.h"
#include "dynlib_optimization.h"
#include "core_math.h"

#define SQR(x) ((x)*(x))

#define NB 32            /* block size for dgels */
#define MINABSTOL 1e-8
#define MAXITERS 100
#define TOLC 1e-5        /* tolerance used for dual infeasibility */
#define SIGTOL 1e-5      /* tolerance used for detecting zero steps
* dF or dZ */
#define MINRCOND 1e-8    /* minimum rcond to declare F_i dependent */


OPTIMIZATION_IMPEXP int sp(int m, int L, double *F, int *blck_szs, double *c,
                           double *x, double *Z, double *ul, double nu, double abstol,
                           double reltol, double tv, int *iters, double *work,
                           int lwork, int *iwork, int *info);

OPTIMIZATION_IMPEXP int C2F(spf)(
    int *m,                /* no of variables */
    int *L,                /* no of blocks in F */
    double *F,            /* F_i's in packed storage */
    int *blck_szs,        /* L-vector, dimensions of diagonal blocks */
    double *c,            /* m-vector */
    double *x,            /* m-vector */
    double *Z,            /* block diagonal matrix in packed storage */
    double *ul,           /* ul[0] = pr. obj, ul[1] = du. obj */
    double *nu,            /* >= 1.0 */
    double *abstol,        /* absolute accuracy */
    double *reltol,        /* relative accuracy */
    double *tv,            /* target value */
    int *iters,           /* on entry: the maximum number of iterations,
				    * on exit: the number of iterations taken */
    double *work,         /* work array */
    int *lwork,            /* size of work */
    int *iwork,           /* work array of m integers */
    int *info            /* status on termination */
);

#endif /* __SPD_H__ */
