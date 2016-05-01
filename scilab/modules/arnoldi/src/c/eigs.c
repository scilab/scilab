/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 -Scilab Enterprises - Adeline CARNIS
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
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "eigs.h"
#include "sci_malloc.h"
#include "sciprint.h"
#include "eigs_dependencies.h"
#include "Sciwarning.h"

/*--------------------------------------------------------------------------*/
extern int C2F(dsaupd)(int *ido, char *bmat, int *n, char *which, int *nev,
                       double *tol, double *resid, int *ncv, double *v,
                       int *ldv, int *iparam, int *ipntr, double *workd,
                       double *workl, int *lworkl, int *info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(dseupd)(int *rvec, char *howmny, int *select, double *d,
                       double *z, int *ldz, double *sigma, char *bmat,
                       int *n, char *which, int *nev , double *tol,
                       double *resid, int *ncv, double *v , int *ldv,
                       int *iparam, int *ipntr, double *workd, double *workl,
                       int *lworkl, int *info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(dnaupd)(int *ido, char *bmat, int *n, char *which, int *nev,
                       double *tol, double *resid, int *ncv, double *v,
                       int *ldv, int *iparam, int *ipntr, double *workd,
                       double *workl, int *lworkl, int *info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(dneupd)(int *rvec, char *howmny, int *select, double *dr,
                       double *di, double *z, int *ldz, double *sigmar,
                       double *sigmai, double *workev, char *bmat, int *n,
                       char *which, int *nev, double *tol, double *resid,
                       int *ncv, double *v, int *ldv, int *iparam, int *ipntr,
                       double *workd, double *workl, int *lworkl, int *info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(znaupd)(int * ido, char * bmat, int * n, char * which,
                       int * nev, double * tol, doublecomplex * resid,
                       int * ncv, doublecomplex * v, int * ldv, int * iparam,
                       int * ipntr, doublecomplex * workd,
                       doublecomplex * workl, int * lworkl, double * rwork,
                       int * info);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(zneupd)(int * rvec, char * howmny, int * select,
                       doublecomplex * d, doublecomplex * z, int * ldz,
                       doublecomplex * sigma, doublecomplex * workev,
                       char * bmat, int * n, char * which, int * nev,
                       double *  tol, doublecomplex * resid, int * ncv,
                       doublecomplex * v, int * ldv, int * iparam, int * ipntr,
                       doublecomplex * workd, doublecomplex * workl,
                       int * lworkl, double * rwork, int * info);
/*--------------------------------------------------------------------------*/

static double alpha = 1.;
static double beta = 0.;

static doublecomplex alphac = {1., 0.};
static doublecomplex betac = {0., 0.};

int eigs(double *AR, doublecomplex *AC, int N, int Acomplex, int Asym,
         double* B, doublecomplex* BC, int Bcomplex, int matB, int nev,
         doublecomplex SIGMA, char* which, double* maxiter, double* tol,
         double* NCV, double* RESID, doublecomplex* RESIDC, int* INFO,
         double* cholB, int INFO_EUPD, double* eigenvalue,
         double* eigenvector, doublecomplex* eigenvalueC,
         doublecomplex* eigenvectorC, int RVEC)

{

    // GENERAL VARIABLES
    int i			= 0;
    int j			= 0;
    int	k			= 0;
    int	l			= 0;
    int INFO_CHOL	= 0;
    int INFO_LU		= 0;
    int INFO_INV    = 0;
    int iOne		= 1;

    // VARIABLES DSAUPD, DNAUPD, ZNAUPD
    int LWORKL		= 0;
    int IDO			= 0;
    int LDV			= Max(1, N);
    int ncv			= 0;

    int IPARAM[11];
    int IPNTR[14];

    double* V			= NULL;
    doublecomplex* VC	= NULL;

    double* WORKD			= NULL;
    doublecomplex* WORKDC	= NULL;

    double* WORKL			= NULL;
    doublecomplex* WORKLC	= NULL;

    double* RWORK			= NULL;

    char* bmat	= "I";

    // VARIABLES DSEUPD, DNEUPD, ZNEUPD
    char* HOWMNY		= "A";

    int* SELECT			= NULL;

    double* DI			= NULL;
    double* DR			= NULL;
    double* Z           = NULL;

    double* WORKEV			= NULL;
    doublecomplex* WORKEVC	= NULL;

    doublecomplex mSIGMA = { -SIGMA.r, -SIGMA.i };

    double* R         = NULL;
    doublecomplex* RC = NULL;

    double* AMSB			= NULL;
    doublecomplex* AMSBC	= NULL;

    int* IPVT	= NULL;

    double* temp = NULL;
    doublecomplex* tempC = NULL;

    int oldnev = nev;
    int N2 = N * N;

    IPARAM[0] = 1;
    IPARAM[2] = (int) maxiter[0];
    IPARAM[6] = 1; // by default mode = 1

    // END VARIABLES

    // MODE
    if (!strcmp(which, "SM") || (SIGMA.r != 0 || SIGMA.i != 0))
    {
        IPARAM[6] = 3;
        which = "LM";
    }

    // BMAT
    if ((matB == 0) || (IPARAM[6] == 1))    // if B = [] or mode = 1 -> bmat = 'I' : standard eigenvalue problem
    {
        bmat = "I";
    }
    else   // generalized eigenvalue problem
    {
        bmat = "G";
    }

    // NCV
    if (NCV == NULL)
    {
        if (Asym == 0 && !Acomplex && !Bcomplex) // if dnaupd  ncv = 2*nev+1
        {
            ncv = Max(2 * nev + 1, 20);
        }
        else // if dsaupd or znaupd ncv = 2*nev
        {
            ncv = Max(2 * nev, 20);
        }
        if (ncv > N)
        {
            ncv = N;
        }
    }
    else
    {
        ncv = (int) NCV[0];
        if (ncv <= nev || ncv > N) // Error
        {
            return -1;
        }
    }

    // NEV
    if ((!Acomplex && !Bcomplex && Asym == 1 && nev >= N) || ((Acomplex || Bcomplex || !Asym) && nev >= N - 1))
    {
        return -2;
    }

    if (matB != 0)
    {
        if (cholB[0]) // we already have the cholesky decomposition
        {
            R = B;
            RC = BC;
        }
        else
        {
            if (IPARAM[6] == 1)
            {
                if (!Bcomplex) // B is real
                {
                    R = (double *)malloc(N * N * sizeof(double));
                    memcpy(R, B, N * N * sizeof(double));
                    C2F(dpotrf) ("u", &N, R, &N, &INFO_CHOL);   // Compute the upper triangular matrix R
                    if (INFO_CHOL != 0) // Errors
                    {
                        free(R);
                        return -3;
                    }
                }
                else	// B is complex
                {
                    RC = (doublecomplex *) malloc(N * N * sizeof(doublecomplex));
                    memcpy(RC, BC, N * N * sizeof(doublecomplex));
                    C2F(zpotrf) ("u", &N, RC, &N, &INFO_CHOL);  // Computes the upper triangular matrix
                    if (INFO_CHOL != 0)
                    {
                        free(RC);
                        return -3;
                    }
                }
            }
        }
    }

    // MAIN
    if (!Acomplex && !Bcomplex)		// A and B are not complex
    {
        if (IPARAM[6] == 3)	// if mode = 3
        {
            AMSB = (double*)malloc(N * N * sizeof(double));
            memcpy(AMSB, AR, N * N * sizeof(double));
            if (SIGMA.r != 0)
            {
                // Compute LU decomposition AMSB = A - sigma*B
                if (matB == 0) // if B = [] -> standard eigenvalue problem : A - sigma *I
                {
                    for (i = 0 ; i < N ; i++)
                    {
                        AMSB[i + i * N] -= SIGMA.r;
                    }
                }
                else	// generalized eigenvalue problem
                {
                    if (cholB[0])
                    {
                        C2F(dsyrk) ("u", "t", &N, &N, &mSIGMA.r, R, &N, &alpha, AMSB, &N);
                        if (!Asym)  //dsyrk does a symmetric update so we need to correct for the antisymmetric part
                        {
                            for (i = 0; i < N; i++)
                            {
                                for (j = 0; j < i; j++)
                                {
                                    AMSB[i + j * N] = AMSB[j + i * N] + AR[i + j * N] - AR[j + i * N];
                                }
                            }
                        }
                    }
                    else
                    {
                        C2F(daxpy)(&N2, &mSIGMA.r, B, &iOne, AMSB, &iOne);
                    }
                }
            }

            // LU decomposition
            IPVT = (int*) calloc(N, sizeof(int));
            C2F(dgetrf)(&N, &N, AMSB, &N, IPVT, &INFO_LU);
            if (INFO_LU > 0)
            {
                free(IPVT);
                free(AMSB);
                return -7;
            }
        }

        if (Asym) // DSAUPD
        {
            LWORKL = ncv * ncv + 8 * ncv;
            WORKL = (double*) calloc(LWORKL, sizeof(double));

        }
        else	// DNAUPD
        {
            LWORKL = 3 * ncv * (ncv + 2);
            WORKL = (double*) calloc(LWORKL, sizeof(double));

        }

        WORKD = (double*) calloc(3 * N, sizeof(double));
        V = (double*) calloc(N * ncv, sizeof(double));

        if (IPARAM[6] == 1 && matB)
        {
            temp = (double*) malloc(N * sizeof(double));
        }

        while (IDO != 99)
        {
            if (Asym) // DSAUPD
            {
                C2F(dsaupd)(&IDO, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO[0]);
            }
            else	// DNAUPD
            {
                C2F(dnaupd)(&IDO, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO[0]);
            }

            if (INFO[0] == -1) //non critical error
            {
                Sciwarning("%s: WARNING: Maximum number of iterations reached. Only %d eigenvalues converged.\n", "eigs", IPARAM[4]);
                break;
            }
            else
            {
                if (INFO[0] < 0)
                {
                    if (R != B)
                    {
                        free(R);
                    }
                    free(IPVT);
                    free(AMSB);
                    free(WORKD);
                    free(WORKL);
                    free(V);
                    free(temp);

                    return -4;
                }
            }

            if (IDO == -1 || IDO == 1 || IDO == 2)
            {
                if (IPARAM[6] == 1) // mode = 1
                {
                    if (IDO == 2)
                    {
                        memcpy(WORKD + IPNTR[1] - 1, WORKD + IPNTR[0] - 1, N * sizeof(double));
                    }
                    else        //IDO=1 or IDO=-1
                    {
                        if (matB == 0) // B = [] -> standard eigenvalue problem
                        {
                            // OP = A*x
                            if (Asym)
                            {
                                C2F(dsymv) ("u", &N, &alpha, AR, &N, WORKD + IPNTR[0] - 1, &iOne, &beta, WORKD + IPNTR[1] - 1, &iOne);
                            }
                            else
                            {
                                C2F(dgemv) ("n", &N, &N, &alpha, AR, &N, WORKD + IPNTR[0] - 1, &iOne, &beta, WORKD + IPNTR[1] - 1, &iOne);
                            }
                        }
                        else // generalized eigenvalue problem
                        {
                            // OP = inv(Rprime)*A*inv(R)*x
                            memcpy(WORKD + IPNTR[1] - 1, WORKD + IPNTR[0] - 1, N * sizeof(double));
                            C2F(dtrsm) ("l", "u", "n", "n", &N, &iOne, &alpha, R, &N, WORKD + IPNTR[1] - 1, &N);
                            memcpy(temp, WORKD + IPNTR[1] - 1, N * sizeof(double));
                            if (Asym)
                            {
                                C2F(dsymv) ("u", &N, &alpha, AR, &N, temp, &iOne, &beta, WORKD + IPNTR[1] - 1, &iOne);
                            }
                            else
                            {
                                C2F(dgemv) ("n", &N, &N, &alpha, AR, &N, temp, &iOne, &beta, WORKD + IPNTR[1] - 1, &iOne);
                            }
                            C2F(dtrsm) ("l", "u", "t", "n", &N, &iOne, &alpha, R, &N, WORKD + IPNTR[1] - 1, &N);
                        }
                    }
                }
                else
                {
                    if (IPARAM[6] == 3) // mode = 3
                    {
                        if (matB == 0) // B = [] -> standard eigenvalue problem
                        {
                            if (IDO == 2)
                            {
                                // y = B*x where B = I so workd[ipntr[1]-1:ipntr[1]+N-1] = workd[ipntr[0]-1:ipntr[0]+N-1]
                                memcpy(WORKD + IPNTR[1] - 1, WORKD + IPNTR[0] - 1, N * sizeof(double));
                            }
                            else
                            {
                                // workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[0]-1:ipntr[0]+N-1]
                                memcpy(WORKD + IPNTR[1] - 1, WORKD + IPNTR[0] - 1, N * sizeof(double));
                                C2F(dgetrs) ("n", &N, &iOne, AMSB, &N, IPVT, WORKD + IPNTR[1] - 1, &N, &INFO_INV);
                            }
                        }
                        else  // matB == 1 so B is not empty and bmat = 'G'-> generalized eigenvalue problem
                        {
                            if (IDO == 2 || IDO == -1)
                            {
                                if (cholB[0])   // workd[ipntr[1]-1:ipntr[1]+N-1] = Rprime * R * workd[ipntr[0]-1:ipntr[0]+N-1]
                                {
                                    memcpy(WORKD + IPNTR[1] - 1, WORKD + IPNTR[0] - 1, N * sizeof(double));
                                    C2F(dtrmv) ("u", "n", "n", &N, B, &N, WORKD + IPNTR[1] - 1, &iOne);
                                    C2F(dtrmv) ("u", "t", "n", &N, B, &N, WORKD + IPNTR[1] - 1, &iOne);
                                }
                                else	//  workd[ipntr[1]-1:ipntr[1]+N-1] = B * workd[ipntr[0]-1:ipntr[0]+N-1]
                                {
                                    C2F(dgemv) ("n", &N, &N, &alpha, B, &N, WORKD + IPNTR[0] - 1, &iOne, &beta, WORKD + IPNTR[1] - 1, &iOne);
                                }
                            }

                            if (IDO == -1)
                            {
                                // compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[1]-1:ipntr[1]+N-1]
                                C2F(dgetrs) ("n", &N, &iOne, AMSB, &N, IPVT, WORKD + IPNTR[1] - 1, &N, &INFO_INV);
                            }
                            else
                            {
                                if (IDO == 1)
                                {
                                    // computes workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[2]-1:ipntr[2]+N-1]
                                    memcpy(WORKD + IPNTR[1] - 1, WORKD + IPNTR[2] - 1, N * sizeof(double));
                                    C2F(dgetrs) ("n", &N, &iOne, AMSB, &N, IPVT, WORKD + IPNTR[1] - 1, &N, &INFO_INV);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (R != B)
                        {
                            free(R);
                        }
                        free(AMSB);
                        free(IPVT);
                        free(WORKD);
                        free(WORKL);
                        free(V);
                        free(temp);

                        return -5;
                    }
                }
            }
        } // END WHILE
        free(AMSB);
        free(IPVT);
        free(temp);
        SELECT = (int *)calloc(ncv, sizeof(int));

        if (Asym) // DSEUPD
        {
            C2F(dseupd) (&RVEC, HOWMNY, SELECT, eigenvalue, eigenvector, &LDV,
                         &SIGMA.r, bmat, &N, which, &nev, tol, RESID, &ncv, V,
                         &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO_EUPD);

            if (INFO_EUPD != 0)
            {
                if (R != B)
                {
                    free(R);
                }
                free(WORKD);
                free(WORKL);
                free(V);
                free(SELECT);
                return -6;
            }
            else
            {
                if (RVEC)
                {
                    if (matB && IPARAM[6] == 1)
                    {
                        // we need to revert back to the original problem
                        // since we really solved for (y,\lambda) in R^{-T}Ay=\lambda y
                        //with y = Rx, so that x = R^{-1}y
                        C2F(dtrsm) ("l", "u", "n", "n", &N, &nev, &alpha, R, &N, eigenvector, &N);
                    }
                }
            }
        }
        else	// DNEUPD
        {
            DR = (double *)calloc((nev + 1), sizeof(double));
            DI = (double *)calloc((nev + 1), sizeof(double));
            WORKEV = (double *)calloc(3 * ncv, sizeof(double));

            RVEC = RVEC || (IPARAM[6] == 3 && SIGMA.i != 0);

            if (RVEC)
            {
                Z = (double *)calloc(N * (nev + 1), sizeof(double));
            }

            C2F(dneupd) (&RVEC, HOWMNY, SELECT, DR, DI, Z, &LDV, &SIGMA.r,
                         &SIGMA.i, WORKEV, bmat, &N, which, &nev, tol, RESID,
                         &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO_EUPD);

            if (INFO_EUPD != 0)
            {
                if (R != B)
                {
                    free(R);
                }
                free(WORKD);
                free(WORKL);
                free(V);
                free(DR);
                free(DI);
                free(Z);
                free(WORKEV);
                free(SELECT);
                return -6;
            }
            else
            {
                if (Z && matB && IPARAM[6] == 1)
                {
                    // we need to revert back to the original problem
                    // since we really solved for (y,\lambda) in R^{-T}Ay=\lambda y
                    //with y = Rx, so that x = R^{-1}y
                    C2F(dtrsm) ("l", "u", "n", "n", &N, &nev, &alpha, R, &N, Z, &N);
                }
                //we use oldnev, because dneupd increases nev by one sometimes.
                process_dneupd_data(DR, DI, Z, N, oldnev, AR, eigenvalueC,
                                    eigenvectorC, (IPARAM[6] == 3) && (SIGMA.i != 0));

                free(DR);
                free(DI);
                free(Z);
                free(WORKEV);
            }
        }

        free(V);
        free(WORKD);
        free(WORKL);
        free(SELECT);
        if (R != B)
        {
            free(R);
        }
    }
    else // A or/and B complex
    {
        if (IPARAM[6] == 3)	// mode = 3
        {
            AMSBC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));

            if (SIGMA.r != 0 || SIGMA.i != 0)
            {
                if (matB == 0)	// standard eigenvalue problem
                {
                    memcpy(AMSBC, AC, N * N * sizeof(doublecomplex));
                    for (i = 0 ; i < N ; i++)
                    {
                        AMSBC[i + i * N].r -= SIGMA.r;
                        AMSBC[i + i * N].i -= SIGMA.i;
                    }
                }
                else	// generalized eigenvalue problem
                {
                    if (cholB[0])
                    {
                        memcpy(AMSBC, BC, N * N * sizeof(doublecomplex));
                        C2F(ztrmm)("l", "u", "c", "n", &N, &N, &mSIGMA, BC, &N, AMSBC, &N);
                        C2F(zaxpy)(&N2, &alphac, AC, &iOne, AMSBC, &iOne);
                    }
                    else
                    {
                        memcpy(AMSBC, AC, N * N * sizeof(doublecomplex));
                        C2F(zaxpy) (&N2, &mSIGMA, BC, &iOne, AMSBC, &iOne);
                    }
                }
            }
            else
            {
                memcpy(AMSBC, AC, N * N * sizeof(doublecomplex));
            }

            // LU decomposition
            IPVT = (int*) calloc(N, sizeof(int));
            C2F(zgetrf) (&N, &N, AMSBC, &N, IPVT, &INFO_LU);
            if (INFO_LU > 0)
            {
                free(IPVT);
                free(AMSBC);
                return (-7);
            }
        }
        LWORKL = 3 * ncv * ncv + 5 * ncv;

        VC = (doublecomplex*) calloc(N * ncv, sizeof(doublecomplex));
        WORKLC = (doublecomplex*) calloc(LWORKL, sizeof(doublecomplex));
        WORKDC = (doublecomplex*) calloc(3 * N, sizeof(doublecomplex));
        RWORK = (double*) calloc(ncv, sizeof(double));
        if (IPARAM[6] == 1 && matB)
        {
            tempC = (doublecomplex*) malloc(N * sizeof(doublecomplex));
        }

        while (IDO != 99)
        {
            C2F(znaupd)(&IDO, bmat, &N, which, &nev, tol, RESIDC, &ncv, VC, &LDV, IPARAM, IPNTR, WORKDC, WORKLC, &LWORKL, RWORK, &INFO[0]);

            if (INFO[0] == -1) //non critical error
            {
                Sciwarning("%s: WARNING: Maximum number of iterations reached. Only %d eigenvalues converged.\n", "eigs", IPARAM[4]);
                break;
            }
            else
            {
                if (INFO[0] < 0)
                {
                    if (RC != BC)
                    {
                        free(RC);
                    }
                    free(WORKDC);
                    free(WORKLC);
                    free(VC);
                    free(RWORK);
                    return -4;
                }
            }

            if (IDO == -1 || IDO == 1 || IDO == 2)
            {
                if (IPARAM[6] == 1) // mode = 1
                {
                    if (IDO == 2)
                    {
                        memcpy(WORKDC + IPNTR[1] - 1, WORKDC + IPNTR[0] - 1, N * sizeof(doublecomplex));
                    }
                    else
                    {
                        if (matB == 0) // B = I
                        {
                            // OP = A*x
                            C2F(zgemv) ("n", &N, &N, &alphac, AC, &N, WORKDC + IPNTR[0] - 1, &iOne, &betac, WORKDC + IPNTR[1] - 1, &iOne);
                        }
                        else
                        {
                            // OP = inv(RC')*A*inv(RC)*x
                            memcpy(WORKDC + IPNTR[1] - 1, WORKDC + IPNTR[0] - 1, N * sizeof(doublecomplex));
                            C2F(ztrsm) ("l", "u", "n", "n", &N, &iOne, &alphac, RC, &N, WORKDC + IPNTR[1] - 1, &N);
                            memcpy(tempC, WORKDC + IPNTR[1] - 1, N * sizeof(doublecomplex));
                            C2F(zgemv) ("n", &N, &N, &alphac, AC, &N, tempC, &iOne, &betac, WORKDC + IPNTR[1] - 1, &iOne);
                            C2F(ztrsm) ("l", "u", "c", "n", &N, &iOne, &alphac, RC, &N, WORKDC + IPNTR[1] - 1, &N);
                        }
                    }
                }
                else
                {
                    if (IPARAM[6] == 3) // if mode = 3
                    {
                        if (matB == 0)	// B = [] -> matB is empty -> standard eigenvalue problem
                        {
                            if (IDO == 2)
                            {
                                // y = B*x where B = I so workd[ipntr[1]-1:ipntr[1]+N-1] = workd[ipntr[0]-1:ipntr[0]+N-1]
                                memcpy(WORKDC + IPNTR[1] - 1, WORKDC + IPNTR[0] - 1, N * sizeof(doublecomplex));
                            }
                            else
                            {
                                // workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[0]-1:ipntr[0]+N-1]
                                memcpy(WORKDC + IPNTR[1] - 1, WORKDC + IPNTR[0] - 1, N * sizeof(doublecomplex));
                                C2F(zgetrs) ("n", &N, &iOne, AMSBC, &N, IPVT, WORKDC + IPNTR[1] - 1, &N, &INFO_INV);
                            }

                        }
                        else  // matB == 1 so B is not empty and bmat = 'G'-> generalized eigenvalue problem
                        {
                            if (IDO == 2 || IDO == -1)
                            {
                                if (cholB[0])   // workd[ipntr[1]-1:ipntr[1]+N-1] = RCprime * RC * workd[ipntr[0]-1:ipntr[0]+N-1]
                                {
                                    memcpy(WORKDC + IPNTR[1] - 1, WORKDC + IPNTR[0] - 1, N * sizeof(doublecomplex));
                                    C2F(ztrmv) ("u", "n", "n", &N, BC, &N, WORKDC + IPNTR[1] - 1, &iOne);
                                    C2F(ztrmv) ("u", "c", "n", &N, BC, &N, WORKDC + IPNTR[1] - 1, &iOne);
                                }
                                else	// workd[ipntr[1]-1:ipntr[1]+N-1] = B *workd[ipntr[0]-1:ipntr[0]+N-1]
                                {
                                    C2F(zgemv) ("n", &N, &N, &alphac, BC, &N, WORKDC + IPNTR[0] - 1, &iOne, &betac, WORKDC + IPNTR[1] - 1, &iOne);
                                }
                            }
                            if (IDO == -1)
                            {
                                // compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[1]-1:ipntr[1]+N-1]
                                C2F(zgetrs) ("n", &N, &iOne, AMSBC, &N, IPVT, WORKDC + IPNTR[1] - 1, &N, &INFO_INV);
                            }
                            else
                            {
                                if (IDO == 1)
                                {
                                    /* compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[2]-1:ipntr[2]+N-1] */
                                    memcpy(WORKDC + IPNTR[1] - 1, WORKDC + IPNTR[2] - 1, N * sizeof(doublecomplex));
                                    C2F(zgetrs) ("n", &N, &iOne, AMSBC, &N, IPVT, WORKDC + IPNTR[1] - 1, &N, &INFO_INV);
                                }
                            }
                        }       //END mode3
                    }
                    else
                    {
                        if (RC != BC)
                        {
                            free(RC);
                        }
                        free(WORKDC);
                        free(WORKLC);
                        free(VC);
                        free(RWORK);
                        free(tempC);

                        return -5;
                    }
                }
            }
        } // END WHILE
        free(tempC);
        free(IPVT);
        free(AMSBC);

        SELECT = (int *)calloc(ncv, sizeof(int));
        WORKEVC = (doublecomplex *) calloc(2 * ncv, sizeof(doublecomplex));

        C2F(zneupd) (&RVEC, HOWMNY, SELECT, eigenvalueC, eigenvectorC, &LDV, &SIGMA, WORKEVC, bmat, &N,
                     which, &nev, tol, RESIDC, &ncv, VC, &LDV, IPARAM, IPNTR, WORKDC,
                     WORKLC, &LWORKL, RWORK, &INFO_EUPD);

        if (INFO_EUPD != 0)
        {
            if (RC != BC)
            {
                free(RC);
            }
            free(WORKDC);
            free(WORKLC);
            free(VC);
            free(SELECT);
            free(WORKEVC);
            free(RWORK);

            return -6;
        }
        else
        {
            if (RVEC)
            {
                if (matB && IPARAM[6] == 1)
                {
                    C2F(ztrsm) ("l", "u", "n", "n", &N, &nev, &alphac, RC, &N, eigenvectorC, &N);
                }
            }
        }

        free(SELECT);
        free(WORKEVC);

        free(VC);
        free(WORKDC);
        free(WORKLC);
        free(RWORK);
        if (RC != BC)
        {
            free(RC);
        }
    }

    return 0;
}
