#ifndef __SP_H__
#define __SP_H__

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
