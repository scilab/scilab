/*
 * Copyright (c) 1994 by Lieven Vandenberghe and Stephen Boyd.
 * Permission to use, copy, modify, and distribute this software for
 * any purpose without fee is hereby granted, provided that this entire
 * notice is included in all copies of any software which is or includes
 * a copy or modification of this software and in all copies of the
 * supporting documentation for such software.
 * This software is being provided "as is", without any express or
 * implied warranty.  In particular, the authors do not make any
 * representation or warranty of any kind concerning the merchantability
 * of this software or its fitness for any particular purpose.
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "spd.h"
#include "sciprint.h"
#include "localization.h"
#include "configvariable_interface.h"
#include "numericconstants_interface.h"
/*--------------------------------------------------------------------------*/
/* BLAS 1 */
extern double F2C(dnrm2)( );
extern double F2C(ddot)( );
extern void F2C(dcopy)( );
extern void F2C(daxpy)( );
extern void F2C(dscal)( );

/* BLAS 2 */
extern void F2C(dgemv)( );
extern void F2C(dspmv)( );

/* BLAS 3 */
extern void F2C(dgemm)( );

/* LAPACK */
extern void F2C(dgels)( );
extern void F2C(dspgst)( );
extern void F2C(dspev)( );
extern void F2C(dspgv)( );
extern void F2C(dtrcon)( );
/*--------------------------------------------------------------------------*/
/*
 * if itype = 1, computes C = B*A*B', otherwise, computes C = B'*A*B
 * A and B are nxn with A symmetric.
 *
 * Arguments:
 * @param itype  = 1: compute C = B*A*B'
 *          = any other integer: computes C = B'*A*B
 * @param n      dimension of A and B
 * @param AP     (input) double array of size n*(n+1)2;
 *          the lower triangle of A in packed storage
 * @param B      (input) double array of size n*n;
 * @param CP     (output) double array of size n*(n+1)/2;
 *          the lower triangle of C in packed storage
 * @param temp:  n-array, workspace
 */
/*--------------------------------------------------------------------------*/
static void cngrncb(int itype, int n, double *AP, double *B, double *CP, double *temp)
{

    int j, pos, lngth = n * (n + 1) / 2;
    int int1 = 1;
    double dbl0 = 0.0, dbl1 = 1.0;

    /* C := 0 */
    F2C(dscal)(&lngth, &dbl0, CP, &int1);

    if (itype == 1)
    {

        for (j = 0, pos = 0;  j < n;  pos += n - j, j++)
        {

            /* temp = A*B(j,:)' */
            F2C(dspmv)("L", &n, &dbl1, AP, B + j, &n, &dbl0, temp, &int1);

            /* C(j:n,j) = B(j:n,:)*temp */
            lngth = n - j;
            F2C(dgemv)("N", &lngth, &n, &dbl1, B + j, &n, temp, &int1, &dbl0,
                       CP + pos, &int1);

        }

    }
    else
    {

        for (j = 0, pos = 0;  j < n;  pos += n - j, j++)
        {

            /* temp = A*B(:,j) */
            F2C(dspmv)("L", &n, &dbl1, AP, B + j * n, &int1, &dbl0, temp, &int1);

            /* C(j:n,j) = B(:,j:n)'*temp */
            lngth = n - j;
            F2C(dgemv)("T", &n, &lngth, &dbl1, B + j * n, &n, temp, &int1, &dbl0,
                       CP + pos, &int1);

        }
    }

}
/*--------------------------------------------------------------------------*/
static double inprd(double *X, double *Z, int L, int *blck_szs)
/*
 * Computes Tr X*Z
 *
 * Arguments:
 * X,Z:       block diagonal matrices with L blocks X^0, ..., X^{L-1},
 *            and Z^0, ..., Z^{L-1}.  X^j and Z^j have size
 *            blck_szs[j] times blck_szs[j].  Every block is stored
 *            using packed storage of the lower triangle.
 * L:         number of blocks
 * blck_szs:  int vector of length L
 *            blck_szs[i], i=0,...,L-1 is the size of block i
 *
 */

{
    double result;
    int i, j, k, lngth, pos, sz, int1 = 1;

    /* sz = length of Z and X */
    for (i = 0, sz = 0;  i < L;  i++)
    {
        sz += (blck_szs[i] * (blck_szs[i] + 1)) / 2;
    }

    /* result = Tr X Z + contributions of diagonal elements */
    result = 2.0 * F2C(ddot)(&sz, X, &int1, Z, &int1);

    /* correct for diagonal elements
     * loop over blocks, j=0,...,L-1  */
    for (j = 0, pos = 0;  j < L;  j++)

        /* loop over columns, k=0,...,blck_szs[j]-1
         * pos is position of (k,k) element of block j
         * lngth is length of column k */
        for (k = 0, lngth = blck_szs[j];  k < blck_szs[j];  pos += lngth,
                lngth -= 1, k++)

            /* subtract Z^j_{kk}*X^j_{kk} from result */
        {
            result -= Z[pos] * X[pos];
        }

    return result;
}
/*--------------------------------------------------------------------------*/
int C2F(spf)(
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
)
{
    return (sp(*m, *L, F, blck_szs, c, x, Z, ul, *nu, *abstol, *reltol, *tv, iters, work,
               *lwork, iwork, info));
}
/*--------------------------------------------------------------------------*/
int sp(
    int m,                /* no of variables */
    int L,                /* no of blocks in F */
    double *F,            /* F_i's in packed storage */
    int *blck_szs,        /* L-vector, dimensions of diagonal blocks */
    double *c,            /* m-vector */
    double *x,            /* m-vector */
    double *Z,            /* block diagonal matrix in packed storage */
    double *ul,           /* ul[0] = pr. obj, ul[1] = du. obj */
    double nu,            /* >= 1.0 */
    double abstol,        /* absolute accuracy */
    double reltol,        /* relative accuracy */
    double tv,            /* target value */
    int *iters,           /* on entry: the maximum number of iterations,
			      * on exit: the number of iterations taken */
    double *work,         /* work array */
    int lwork,            /* size of work */
    int *iwork,           /* work array of m integers */
    int *info             /* status on termination */
)
/*
 * Solves semidefinite program
 *
 *  minimize    c'*x
 *  subject to  F_0 + x_1*F_1 + ... + x_m*F_m  >= 0
 *
 * and its dual
 *
 *  maximize    -Tr F_0*Z
 *  subject to  Z >= 0
 *              Tr F_i*Z = c_i, i=1,...,m
 *
 *
 * Convergence criteria:
 * (1) maxiters is exceeded
 * (2) duality gap is less than abstol
 * (3) primal and dual objective are both positive and
 *     duality gap is less than reltol * dual objective
 *     or primal and dual objective are both negative and
 *     duality gap is less than reltol * minus the primal objective
 * (4) reltol is negative and primal objective is less than tv
 * (5) reltol is negative and dual objective is greater than tv
 *
 * Arguments:
 * - m:        number of variables x_i. m >= 1.
 * - L:        number of diagonal blocks in F_i. L >= 1.
 * - F:        the block diagonal matrices F_i, i=0,...,m.
 *             it is assumed that the matrices F_i are linearly
 *             independent.
 *             let F_i^j, i=0,..,m, j=0,...,L-1 denote the jth
 *             diagonal block of F_i,
 *             the array F contains F_0^0, ..., F_0^{L-1}, F_1^0, ...,
 *             F_1^{L-1}, ..., F_m^0, ..., F_m^{L-1}, in this order,
 *             using packed storage for the lower triangular part of
 *             F_i^j.
 * - blck_szs: an int L-vector. blck_szs[j], j=0,....L-1 gives the
 *             size of block j, ie, F_i^j has size blck_szs[j]
 *             times blck_szs[j].
 * - c:        m-vector, primal objective.
 * - x:        m-vector.  On entry, a strictly primal feasible point.
 *             On exit, the last iterate for x.
 * - Z:        block diagonal matrix with L blocks Z^0, ..., Z^{L-1}.
 *             Z^j has size blck_szs[j] times blck_szs[j].
 *             Every block is stored using packed storage of the lower
 *             triangular part.
 *             On entry, a strictly dual feasible point.  On exit, the
 *             last dual iterate.
 * - ul:       two-vector.  On exit, ul[0] is the primal objective value
 *             c'*x;  ul[1] is the dual objective value -Tr F_0*Z.
 * - nu:       >= 1.0. Controls the rate of convergence.
 * - abstol:   absolute tolerance, >= MINABSTOL.
 * - reltol:   relative tolerance.  Has a special meaning when negative.
 * - tv:       target value, only referenced if reltol < 0.
 * - iters:    on entry: maximum number of iterations >= 0,
 *             on exit: the number of iterations taken.
 * - work:     work array of size lwork.
 * - lwork:    size of work, must be at least:
 *             (m+2)*sz + up_sz + 2*n + ltemp, with
 *             ltemp = max( m+sz*nb, 3max_n + max_n*(max_n+1), 3*m )
 *             (sz: space needed to store one matrix F_i in packed
 *             storage, ie,
 *                sum_{j=0}^{L-1} blck_szs[j]*(blck_szs[j]+1)/2;
 *             up_sz: space needed to store one matrix F_i in
 *             unpacked storage, ie,
 *                sum_{j=0}^{L-1} blck_szs[j]*blck_szs[j];
 *             max_n: max block size;
 *             n: sum of the block sizes.
 *             nb >= 1, for best performance, nb should be at least
 *             equal to the optimal block size for dgels.
 * - iwork:    work array of m integers
 * - info:     returns 1 if maxiters exceeded,  2 if absolute accuracy
 *             is reached, 3 if relative accuracy is reached,
 *             4 if target value is reached, 5 if target value is
 *             not achievable;
 *             negative values indicate errors: -i means argument i
 *             has an illegal value, -18 stands for all other errors.
 *
 *
 * Returns 0 for normal exit, 1 if an error occurred.
 *
 */


{
    int i, j, k, n, sz, up_sz, max_n, lngth, pos, pos2, pos3, pos4, ltemp,
        maxiters, info2, minlwork;
    double q, *rhs, *Fsc, *R, *X, rho, *dx, *sigx, *sigz, *dZ, *temp, scal,
           scal2, XdZ, ZdX, alphax, alphaz, lambda_ls, gradx, hessx,
           gradz, hessz, dalphax, dalphaz, gap, newgap = 0.0, newu = 0.0,
                                                newl = 0.0, maxpossigx, minnegsigx, maxpossigz, minnegsigz, nrmc,
                                                nrmx, nrmz, nrmmax, rcond;
    int int2 = 2, int1 = 1;
    double dbl1 = 1.0, dbl0 = 0.0, sqrt2 = sqrt(2.0);
    double dbl_epsilon;

    if (m < 1)
    {
        if ( getWarningMode() )
        {
            sciprint(_("m must be at least one.\n"));
        }
        *info = -1;
        return 1;
    }
    if (L < 1)
    {
        if ( getWarningMode() )
        {
            sciprint(_("L must be at least one.\n"));
        }
        *info = -2;
        return 1;
    }
    for (i = 0; i < L; i++) if (blck_szs[i] < 1)
        {
            if ( getWarningMode() )
            {
                sciprint(_("blck_szs[%d] must be at least one.\n"), i);
            }
            *info = -4;
            return 1;
        }
    if (nu < 1.0)
    {
        if ( getWarningMode() )
        {
            sciprint(_("nu must be at least 1.0.\n"));
        }
        *info = -9;
        return 1;
    }


    /*
     * calculate dimensions:
     * n:      total size of semidefinite program
     * sz:     length of one block-diagonal matrix in packed storage
     * up_sz:  length of one block-diagonal matrix in unpacked storage
     * max_n:  size of biggest block
     */

    for (i = 0, n = 0, sz = 0, up_sz = 0, max_n = 0;  i < L;  i++)
    {
        n     += blck_szs[i];
        sz    += blck_szs[i] * (blck_szs[i] + 1) / 2;
        up_sz += blck_szs[i] * blck_szs[i];
        max_n  = Max(max_n, blck_szs[i]);
    }
    if (m > sz)
    {
        sciprint(_("Matrices Fi, i=1,...,m are linearly dependent.\n"));
        *info = -3;
        return 1;
    }

    q = (double)n + nu * sqrt((double)n);


    /*
     * check if Tr Fi*Z = c_i, i=1,...,m
     */

    nrmc = F2C(dnrm2)(&m, c, &int1);
    for (i = 0; i < m; i++)
        if (fabs(inprd(F + (i + 1)*sz, Z, L, blck_szs) - c[i]) > nrmc * TOLC)
        {
            if ( getWarningMode() )
            {
                sciprint(_("Z0 does not satisfy equality conditions for dual feasibility.\n"));
            }

            *info = -7;
            return 1;
        }


    /*
     * organize workspace
     *
     * work:  (m+2)*sz + up_sz + 2*n + ltemp
     * minimum ltemp: the maximum of
     *         m+sz*nb, 3*max_n + max_n*(max_n+1), and 3*m
     *         (nb is at least one)
     *
     * for dgels:        m + sz*nb, nb at least 1
     * for dspev("N"):   3*max_n + max_n*(max_n+1)
     * for dspgv("N"):   3*max_n + max_n*(max_n+1)
     * for dspgv("V"):   3*max_n + max_n*(max_n+1)/2
     * for cngrncb:      max_n
     * for dtrcon:       3*m
     *
     * rhs  (sz):        work[0 ... sz-1]
     * Fsc  (m*sz):      work[sz ... (m+1)*sz-1]
     * R    (up_sz):     work[(m+1)*sz ... (m+1)*sz+up_sz-1]
     * X    (sz):        work[(m+1)*sz+up_sz ... (m+2)*sz+up_sz-1]
     * sigx (n):         work[(m+2)*sz+up_sz ... (m+2)*sz+up_sz+n-1]
     * sigz (n):         work[(m+2)*sz+up_sz+n ... (m+2)*sz+up_sz+2*n-1]
     * temp (remainder): work[(m+2)*sz+up_sz+2*n ... lwork-1]
     */

    /* check lwork */
    minlwork = (m + 2) * sz + up_sz + 2 * n +
               Max( Max( m + sz, 3 * max_n + max_n * (max_n + 1) ), 3 * m );
    if (lwork < minlwork)
    {
        if ( getWarningMode() )
        {
            sciprint(_("Work space is too small.  Need at least %d*sizeof(double).\n"), minlwork);
        }
        *info = -15;
        return 1;
    }

    rhs   = work;        /* rhs for ls problem */
    dx    = work;        /* solution of ls system; overlaps with rhs  */
    Fsc   = rhs + sz;    /* scaled matrices */
    dZ    = rhs + sz;    /* overlaps with first column of Fsc */
    R     = Fsc + m * sz; /* eigenvectors of Z*F */
    X     = R + up_sz;   /* F(x) */
    sigx  = X + sz;      /* generalized eigenvalues of (dX,X) */
    sigz  = sigx + n;    /* generalized eigenvalues of (dZ,Z) */
    temp  = sigz + n;
    ltemp = lwork - (m + 2) * sz - up_sz - 2 * n;


    maxiters = (*iters >= 0) ? *iters : MAXITERS;
    for (*iters = 0; *iters <= maxiters; (*iters)++)
    {


        /* compute F(x) = F_0 + x_1*F_1 + ... + x_m*F_m, store in X */
        F2C(dcopy)(&sz, F, &int1, X, &int1);
        F2C(dgemv)("N", &sz, &m, &dbl1, F + sz, &sz, x, &int1, &dbl1, X, &int1);


        /*
         * compute generalized eigendecomp  Z*F*x = lambda*x
         * loop over blocks, i=0,...,L-1
         * pos:  position of (0,0) element of block i in packed storage
         * pos2: position of (0,0) element of block i in unpacked
         *       storage
         * pos3: position of first eigenvalue of block i in sigx
         */

        for (i = 0, pos = 0, pos2 = 0, pos3 = 0, gap = 0.0;  i < L;
                pos += blck_szs[i] * (blck_szs[i] + 1) / 2,
                pos2 += blck_szs[i] * blck_szs[i],
                pos3 += blck_szs[i], i++)
        {

            lngth = blck_szs[i] * (blck_szs[i] + 1) / 2;

            /* copy block i of Z in temp (need max_n*(max_n+1)/2) */
            F2C(dcopy)(&lngth, Z + pos, &int1, temp, &int1);

            /* generalized eigenvalue decomposition Z*F*x = lambda*x
             * - eigenvectors V are normalized s.t. V^T*F*V = I
             * - store block i of V in R+pos2
             * - store eigenvalues of block i in sigx+pos3
             * - dspgv replaces X+pos by cholesky factor L of ith
             *   block of F (F = L*L^T)
             * use temp+lngth as workspace (need at least 3*max_n) */
            F2C(dspgv)(&int2, "V", "L", blck_szs + i, temp, X + pos, sigx + pos3,
                       R + pos2, blck_szs + i, temp + lngth, &info2);
            if (info2)
            {
                if ( getWarningMode() )
                {
                    sciprint(_("Error in dspgv, info = %d.\n"), info2);
                }
                if (*iters == 0 && info2 > blck_szs[i])
                {
                    if ( getWarningMode() )
                    {
                        sciprint( _("x0 is not strictly primal feasible.\n"));
                    }
                    *info = -6;
                }
                else
                {
                    *info = -18;
                }
                return 1;
            }

            /* - replace sigx+pos3 by lambda^(1/2)
             * - normalize block i of V (stored in R+pos2) s.t.
             *   V^T*F*V = Lambda^(1/2) */
            for (k = 0; k < blck_szs[i]; k++)
            {
                scal = sigx[pos3 + k];
                if (scal < 0.0)
                {
                    if (*iters == 0)
                    {
                        if ( getWarningMode() )
                        {
                            sciprint(_("Z0 is not positive definite.\n"));
                        }
                        *info = 7;
                    }
                    else
                    {
                        if ( getWarningMode() )
                        {
                            sciprint(_("F(x)*Z has a negative eigenvalue.\n"));
                        }
                        *info = -18;
                    }
                    return 1;
                }
                gap += scal;    /* duality gap is sum of eigenvalues of ZF */
                scal2 = sqrt(scal);
                scal = sqrt(scal2);
                sigx[pos3 + k] = scal2;
                F2C(dscal)(blck_szs + i, &scal, R + pos2 + k * blck_szs[i], &int1);
            }

        }


        /*
         * check convergence
         */

        ul[1] = -inprd(F, Z, L, blck_szs);      /* -Tr F_0 Z */
        ul[0] = F2C(ddot)(&m, c, &int1, x, &int1);  /* c^T x */
        if (*iters == 0)
        {
            if ( getWarningMode() )
            {
                sciprint(_("\n    primal obj.  dual obj.  dual. gap \n"));
            }

        }
        if ( getWarningMode() )
        {
            sciprint("% 13.2e % 12.2e %10.2e\n", ul[0], ul[1], gap);
        }
        if (gap <= Max(abstol, MINABSTOL))
        {
            *info = 2;
        }
        else if ( (ul[1] > 0.0 && gap <= reltol * ul[1]) ||
                  (ul[0] < 0.0 && gap <= reltol * (-ul[0])) )
        {
            *info = 3;
        }
        else if ( reltol < 0.0 && ul[0] <= tv )
        {
            *info = 4;
        }
        else if ( reltol < 0.0 && ul[1] >= tv )
        {
            *info = 5;
        }
        else if ( *iters == maxiters )
        {
            *info = 1;
        }
        else
        {
            *info = 0;
        }
        if (*info)
        {
            return 0;
        }



        /*
         * compute scaled matrices F
         */

        for (j = 0, pos = 0;  j < m;  j++) for (i = 0, pos2 = 0;  i < L;
                                                    pos += blck_szs[i] * (blck_szs[i] + 1) / 2,
                                                    pos2 += blck_szs[i] * blck_szs[i], i++)
            {

                /* compute R' * Fj(i) * R, store in Fsc+pos */
                cngrncb(2, blck_szs[i], F + sz + pos, R + pos2, Fsc + pos, temp);

                /* correct diagonal elements */
                for (k = 0, pos4 = pos;  k < blck_szs[i];  pos4 += blck_szs[i] - k, k++)
                {
                    Fsc[pos4] /= sqrt2;
                }

            }


        /*
         * form rhs = Lambda^(-1/2) - (q/gap) * Lambda^(1/2)
         */

        F2C(dscal)(&sz, &dbl0, rhs, &int1);    /* rhs := 0 */
        rho = -q / gap;
        for (i = 0, pos = 0, pos3 = 0;  i < L;
                pos += blck_szs[i] * (blck_szs[i] + 1) / 2,
                pos3 += blck_szs[i], i++)
            for (k = 0, pos4 = pos;  k < blck_szs[i];  pos4 += blck_szs[i] - k, k++)
            {
                scal = sigx[pos3 + k];
                rhs[pos4] = (1.0 / scal + rho * scal) / sqrt2;
            }


        /*
         * solve least-squares problem; need workspace of size m + nb*sz
         * - rhs is overwritten by dx
         * - in first iteration, estimate condition number of Fsc
         */

        F2C(dgels)("N", &sz, &m, &int1, Fsc, &sz, rhs, &sz, temp, &ltemp,
                   &info2);
        if (info2)
        {
            if ( getWarningMode() )
            {
                sciprint(_("Error in dgels, info = %d.\n"), info2);
            }
            *info = -18;
            return 1;
        }

        if (*iters == 0)
        {

            /* estimate the condition number in 1-norm of the R-factor of
             * the qr-decomposition of Fsc (is stored in Fsc)
             * need work space of size 3*m */
            F2C(dtrcon)("1", "U", "N", &m, Fsc, &sz, &rcond, temp, iwork,
                        &info2);
            if (info2 < 0)
            {
                if ( getWarningMode() )
                {
                    sciprint(_("Error in dtrcon, info = %d.\n"), info2);
                }
                *info = -18;
                return 1;
            }
            if (rcond < MINRCOND)
            {
                if ( getWarningMode() )
                {
                    sciprint(_("The matrices F_i, i=1,...,m are linearly dependent (or the initial points are very badly conditioned).\n"));
                }
                *info = -3;
                return 1;
            }

        }



        /*
         * - compute dZ =
         *   R*((q/gap)*Lambda^(1/2) - Lambda^(-1/2) + R^T*dF*R )*R^T
         * - compute generalized eigenvalues of (dF, F), store in sigx
         * - compute generalized eigenvalues of (dZ, Z), store in sigz
         *
         * loop over blocks i=0,...,L-1
         * pos:  position of (0,0) element of block i in packed storage
         * pos2: position of (0,0) element of block i in unpacked storage
         * pos3: position of first eigenvalue of in sigx and sigz
         */

        for (i = 0, pos = 0, pos2 = 0, pos3 = 0;  i < L;
                pos  += blck_szs[i] * (blck_szs[i] + 1) / 2,
                pos2 += blck_szs[i] * blck_szs[i],
                pos3 += blck_szs[i], i++)
        {

            lngth = blck_szs[i] * (blck_szs[i] + 1) / 2;

            /* compute ith block of dF = \sum \delta x_i F_i,
             * store in temp */
            F2C(dgemv)("N", &lngth, &m, &dbl1, F + sz + pos, &sz, dx, &int1,
                       &dbl0, temp, &int1);

            /* scale dF as R'*dF*R, store in temp + lngth */
            cngrncb(2, blck_szs[i], temp, R + pos2, temp + lngth, temp + 2 * lngth);

            /* add (q/gap)*Lambda^(1/2) - Lambda^(-1/2) */
            for (k = 0, pos4 = lngth;  k < blck_szs[i];  pos4 += blck_szs[i] - k, k++)
            {
                temp[pos4] -= rho * sigx[pos3 + k] + 1.0 / sigx[pos3 + k];
            }

            /* replace dF in temp by L^{-1}*dF*L^{-T},
             * (L: cholesky factor of F, stored in X)
             * and compute eigenvalues of L^{-1}*dF*L^{-T}  */
            F2C(dspgst)(&int1, "L", blck_szs + i, temp, X + pos, &info2);
            if (info2)
            {
                if ( getWarningMode() )
                {
                    sciprint(_("Error in dspst, info = %d.\n"), info2);
                }

                *info = -18;
                return 1;
            }
            /* temp has to be of size max_n*(max_n+1)+3*max_n */
            F2C(dspev)("N", "L", blck_szs + i, temp, sigx + pos3, NULL, &int1,
                       temp + 2 * lngth, &info2);
            if (info2)
            {
                if ( getWarningMode() )
                {
                    sciprint(_("Error in dspev, info = %d.\n"), info2);
                }
                *info = -18;
                return 1;
            }

            /* dZ := R*((q/gap)*Lambda^(1/2) - Lambda^(-1/2) + R'*dF*R)*R' */
            cngrncb(1, blck_szs[i], temp + lngth, R + pos2, dZ + pos,
                    temp + 2 * lngth);

            /* copy ith block of dZ to temp */
            F2C(dcopy)(&lngth, dZ + pos, &int1, temp, &int1);

            /* copy ith block of Z to temp + lngth */
            F2C(dcopy)(&lngth, Z + pos, &int1, temp + lngth, &int1);

            /* sigz: generalized eigenvalues of (dZ,Z)
             * required size of temp: 3*max_n + max_n*(max_n+1) */
            F2C(dspgv)(&int1, "N", "L", blck_szs + i, temp, temp + lngth, sigz + pos3,
                       NULL, &int1, temp + 2 * lngth, &info2);
            if (info2)
            {
                if ( getWarningMode() )
                {
                    sciprint(_("Error in dspgv, info = %d.\n"), info2);
                }
                *info = -18;
                return 1;
            }

        }


        /*
         * compute feasible rectangle for plane search
         */

        maxpossigx = 0.0;
        minnegsigx = 0.0;
        maxpossigz = 0.0;
        minnegsigz = 0.0;
        for (i = 0; i < n; i++)
        {
            if ( sigx[i] > maxpossigx )
            {
                maxpossigx = sigx[i];    /* max pos eigenvalue in sigx */
            }
            else if ( sigx[i] < minnegsigx )
            {
                minnegsigx = sigx[i];    /* min neg eigenvalue in sigx */
            }
            if ( sigz[i] > maxpossigz )
            {
                maxpossigz = sigz[i];    /* max pos eigenvalue in sigz */
            }
            else if ( sigz[i] < minnegsigz )
            {
                minnegsigz = sigz[i];    /* min neg eigenvalue in sigz */
            }
        }
        nrmx = F2C(dnrm2)(&n, sigx, &int1);        /* norm of scaled dx */
        nrmz = F2C(dnrm2)(&n, sigz, &int1);        /* norm of scaled dZ */
        nrmmax = Max( nrmx, nrmz);

        XdZ = inprd(F, dZ, L, blck_szs);       /* Tr F0*dZ */
        ZdX = F2C(ddot)(&m, c, &int1, dx, &int1);  /* c^T*dx */


        /*
         * check corners of feasible rectangle
         */

        dbl_epsilon = nc_eps();
        if (nrmx > SIGTOL * nrmmax)
            if (ZdX < 0.0)
            {
                alphax = (minnegsigx < -dbl_epsilon) ? -1.0 / minnegsigx : 0.0;
            }
            else
            {
                alphax = (maxpossigx >  dbl_epsilon) ? -1.0 / maxpossigx : 0.0;
            }
        else
        {
            alphax = 0.0;
        }

        if (nrmz > SIGTOL * nrmmax)
            if (XdZ < 0.0)
            {
                alphaz = (minnegsigz < -dbl_epsilon) ? -1.0 / minnegsigz : 0.0;
            }
            else
            {
                alphaz = (maxpossigz >  dbl_epsilon) ? -1.0 / maxpossigz : 0.0;
            }
        else
        {
            alphaz = 0.0;
        }

        newgap = gap + alphax * ZdX + alphaz * XdZ;
        newu = ul[0] + alphax * ZdX;
        newl = ul[1] - alphaz * XdZ;

        if (newgap <= Max(abstol, MINABSTOL))
        {
            *info = 2;
        }
        else if ( (newl > 0.0 && newgap <= reltol * newl) ||
                  (newu < 0.0 && newgap <= -reltol * newu) )
        {
            *info = 3;
        }
        else if ( reltol < 0.0 && newu <= tv )
        {
            *info = 4;
        }
        else if ( reltol < 0.0 && newl >= tv )
        {
            *info = 5;
        }
        else if ( *iters == maxiters )
        {
            *info = 1;
        }
        else
        {
            *info = 0;
        }

        if (*info)
        {
            F2C(daxpy)(&m, &alphax, dx, &int1, x, &int1);
            F2C(daxpy)(&sz, &alphaz, dZ, &int1, Z, &int1);
            gap = newgap;
            ul[0] = newu;
            ul[1] = newl;
            if ( getWarningMode() )
            {
                sciprint("% 13.2e % 12.2e %10.2e\n", ul[0], ul[1], gap);
            }
            (*iters)++;
            return 0;
        }


        /*
         * plane search
         *  minimize   phi(alphax,alphaz) =
         *    q*log(dual_gap + alphax*c^T*dx + alphaz* Tr F_0 dZ)
         *  - sum log (1+alphax*sigx_i) - sum log (1+alphaz*sigz)
         */

        alphax = 0.0;
        alphaz = 0.0;
        lambda_ls = 1.0;

        if (nrmx > SIGTOL * nrmmax)
            if (nrmz > SIGTOL * nrmmax)  /* compute primal and dual steps */
                while ( lambda_ls > 1e-4 )
                {

                    /* compute 1st and 2nd derivatives of phi */
                    rho = q / (gap + alphax * ZdX + alphaz * XdZ);
                    gradx = rho * ZdX;
                    hessx = 0.0;
                    gradz = rho * XdZ;
                    hessz = 0.0;
                    for (i = 0; i < n; i++)
                    {
                        gradx -= sigx[i] / (1.0 + alphax * sigx[i]);
                        hessx += SQR( sigx[i] / (1.0 + alphax * sigx[i]) );
                        gradz -= sigz[i] / (1.0 + alphaz * sigz[i]);
                        hessz += SQR( sigz[i] / (1.0 + alphaz * sigz[i]) );
                    }

                    /* newton step */
                    dalphax = -gradx / hessx;
                    dalphaz = -gradz / hessz;
                    lambda_ls = sqrt( SQR(gradx) / hessx + SQR(gradz) / hessz );
                    alphax += (lambda_ls > 0.25) ?
                              dalphax / (1.0 + lambda_ls) : dalphax;
                    alphaz += (lambda_ls > 0.25) ?
                              dalphaz / (1.0 + lambda_ls) : dalphaz;

                }

            else while ( lambda_ls > 1e-4 )    /* primal step only */
                {

                    /* compute 1st and 2nd derivatives of phi */
                    rho = q / (gap + alphax * ZdX);
                    gradx = rho * ZdX;
                    hessx = 0.0;
                    for (i = 0; i < n; i++)
                    {
                        gradx -= sigx[i] / (1.0 + alphax * sigx[i]);
                        hessx += SQR( sigx[i] / (1.0 + alphax * sigx[i]) );
                    }

                    /* newton step */
                    dalphax = -gradx / hessx;
                    lambda_ls = fabs(gradx) / sqrt(hessx);
                    alphax += (lambda_ls > 0.25) ?
                              dalphax / (1.0 + lambda_ls) : dalphax;

                }

        else if (nrmz > SIGTOL * nrmmax)      /* dual step only */
            while ( lambda_ls > 1e-4 )
            {

                /* compute 1st and 2nd derivatives of phi */
                rho = q / (gap + alphaz * XdZ);
                gradz = rho * XdZ;
                hessz = 0.0;
                for (i = 0; i < n; i++)
                {
                    gradz -= sigz[i] / (1.0 + alphaz * sigz[i]);
                    hessz += SQR( sigz[i] / (1.0 + alphaz * sigz[i]) );
                }

                /* newton step */
                dalphaz = -gradz / hessz;
                lambda_ls = fabs(gradz) / sqrt(hessz);
                alphaz += (lambda_ls > 0.25) ?
                          dalphaz / (1.0 + lambda_ls) : dalphaz;
            }



        /* update x and Z */
        F2C(daxpy)(&m, &alphax, dx, &int1, x, &int1);
        F2C(daxpy)(&sz, &alphaz, dZ, &int1, Z, &int1);

    }

    return -1;   /* should never happen */
}

