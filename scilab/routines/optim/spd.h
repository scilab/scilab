/* Copyright INRIA */
#ifndef __SPD_H__
#define __SPD_H__

#include "../machine.h"

#define SQR(x) ((x)*(x))
#define MAX(x,y)  ((x) > (y) ? (x) : (y))
#define MIN(x,y)  ((x) < (y) ? (x) : (y))

#define NB 32            /* block size for dgels */
#define MINABSTOL 1e-8 
#define MAXITERS 100
#define TOLC 1e-5        /* tolerance used for dual infeasibility */
#define SIGTOL 1e-5      /* tolerance used for detecting zero steps 
                          * dF or dZ */ 
#define MINRCOND 1e-8    /* minimum rcond to declare F_i dependent */

/* BLAS 1 */
double F2C(dnrm2)( );
double F2C(ddot)( );
void F2C(dcopy)( );
void F2C(daxpy)( );
void F2C(dscal)( );

/* BLAS 2 */
void F2C(dgemv)( );
void F2C(dspmv)( );

/* BLAS 3 */
void F2C(dgemm)( );

/* LAPACK */
void F2C(dgels)( );
void F2C(dspgst)( );
void F2C(dspev)( );
void F2C(dspgv)( );
void F2C(dtrcon)( );
double F2C(dlamch)( );

int sp( /* int m, int L, double *F, int *blck_szs, double *c, 
           double *x, double *Z, double *ul, double nu, double abstol, 
           double reltol, double tv, int *iters, double *work, 
           int lwork, int *iwork, int *info */ );

#endif /* __SPD_H__ */
