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

#ifndef __SP_H__
#define __SP_H__

#define SQR(x) ((x)*(x))

#define NB 32            /* block size for dgels */
#define MINABSTOL 1e-8
#define MAXITERS 100
#define TOLC 1e-5        /* tolerance used for dual infeasibility */
#define SIGTOL 1e-5      /* tolerance used for detecting zero steps
* dF or dZ */
#define MINRCOND 1e-8    /* minimum rcond to declare F_i dependent */

/* BLAS 1 */
double dnrm2_( );
double ddot_( );
void dcopy_( );
void daxpy_( );
void dscal_( );

/* BLAS 2 */
void dgemv_( );
void dspmv_( );

/* BLAS 3 */
void dgemm_( );

/* LAPACK */
void dgels_( );
void dspgst_( );
void dspev_( );
void dspgv_( );
void dtrcon_( );

int sp( /* int m, int L, double *F, int *blck_szs, double *c,
           double *x, double *Z, double *ul, double nu, double abstol,
           double reltol, double tv, int *iters, double *work,
           int lwork, int *iwork, int *info */ );

#endif /* __SP_H__ */
