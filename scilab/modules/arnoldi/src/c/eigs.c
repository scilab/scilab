/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 -Scilab Enterprises - Adeline CARNIS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "eigs.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "eigs_dependencies.h"
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
// dgemm performs one of the matrix-matrix operations
extern int C2F(dgemm)(char* transa, char* transb, int* m, int* n, int* k, double* alpha, double* A, int* lda, double* B, int* ldb, double* beta, double* C, int* ldc);

// zgemm performs one of the matrix-matrix operations
extern int C2F(zgemm)(char* transa, char* transb, int* m, int* n, int* k, doublecomplex* alpha, doublecomplex* A, int* lda,
                      doublecomplex* B, int* ldb, doublecomplex* beta, doublecomplex* C, int* ldc);

// dgetrf computes an LU factorization of a general M by N matrix A (double) using partial pivoting with row interchanges
extern int C2F(dgetrf)(int* m, int* n, double* A, int* lda, int* ipiv, int* info);

// zgetrd computes an LU factorization of a general M by N matrix A (complex*16) using partial pivoting with row interchanges
extern int C2F(zgetrf)(int* m, int* n, doublecomplex* A, int* lda, int* ipiv, int* info);

// dlaswp performs a series of row interchanges on the matrix A
// one row interchange is initiated for each of rows k1 through k2 of A
extern int C2F(dlaswp)(int* n, double* A, int* lda, int* k1, int* k2, int* ipiv, int* incx);

// dpotrf computes the cholesky factorization of a real symmetric positive definite matrix A
extern int C2F(dpotrf)(char* uplo, int* n, double* A, int* lda, int* info);

// zpotrf computes the cholesky factorization of a real hermitian positive definite matrix A
extern int C2F(zpotrf)(char* uplo, int* n, doublecomplex* A, int* lda, int* info);

// dgetri computes the inverse of a matrix using the LU factorization computed by dgetrf
extern int C2F(dgetri)(int* n, double* A, int* lda, int* ipiv, double* work, int* lworkl, int* info);

// zgetri computes the inverse of a matrix using the LU factorization computed by zgetrf
extern int C2F(zgetri)(int* n, doublecomplex* A, int* lda, int* ipiv, doublecomplex* work, int* lworkl, int* info);
/*--------------------------------------------------------------------------*/

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
                       int *lworkl, int *info, unsigned long rvec_length,
                       unsigned long howmany_length,
                       unsigned long bmat_length, unsigned long which_len);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
extern int C2F(dnaupd)(int *ido, char *bmat, int *n, char *which, int *nev,
                       double *tol, double *resid, int *ncv, double *v,
                       int *ldv, int *iparam, int *ipntr, double *workd,
                       double *workl, int *lworkl, int *info,
                       unsigned long bmat_len, unsigned long which_len);
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

int eigs(double *AR, doublecomplex *AC, int N, int Acomplex, int Asym, double* B,
         doublecomplex* BC, int Bcomplex, int matB, int nev, doublecomplex* SIGMA,
         char* which, double* maxiter, double* tol, double* NCV, double* RESID, doublecomplex* RESIDC,
         int* INFO, double* cholB, int INFO_EUPD, doublecomplex* eigenvalue, doublecomplex* eigenvector)
{

    int index = 0;
    // GENERAL VARIABLES
    int i			= 0;
    int j			= 0;
    int	k			= 0;
    int	l			= 0;
    int INFO_CHOL	= 0;
    int INFO_LU		= 0;
    int k1			= 1;
    int iOne		= 1;

    // VARIABLES DSAUPD, DNAUPD, ZNAUPD
    int LWORKL		= 0;
    int IDO			= 0;
    int LDV			= Max(1, N);
    int ncv			= 0;

    int* IPARAM		= NULL;
    int* IPNTR		= NULL;

    double* V			= NULL;
    doublecomplex* VC	= NULL;

    double* WORKD			= NULL;
    doublecomplex* WORKDC	= NULL;

    double* WORKL			= NULL;
    doublecomplex* WORKLC	= NULL;

    double* RWORK			= NULL;

    char* bmat	= "I";

    // VARIABLES DSEUPD, DNEUPD, ZNEUPD
    int RVEC			= 0;	// compute eigenvalues if RVEC = 1 also compute eigenvalues and eigenvectors
    char* HOWMNY		= "A";

    int* SELECT			= NULL;

    double* D			= NULL;
    double* DI			= NULL;
    double* DR			= NULL;
    doublecomplex* DC	= NULL;

    double* WORKEV			= NULL;
    doublecomplex* WORKEVC	= NULL;

    double* Z			= NULL;
    doublecomplex* ZC	= NULL;

    double SIGMAR		= SIGMA[0].r;
    double SIGMAI		= SIGMA[0].i;

    double* R = (double*)malloc(N * N * sizeof(double));
    double* Rprime = (double*)malloc(N * N * sizeof(double));

    double* AMSB			= NULL;
    doublecomplex* AMSBC	= NULL;

    double* L	= NULL;
    double* U	= NULL;
    double* E	= NULL;

    doublecomplex* RC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
    doublecomplex* RCprime = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));

    doublecomplex* LC	= NULL;
    doublecomplex* UC	= NULL;
    doublecomplex* EC	= NULL;

    double* tmp_WORKD	= NULL;
    doublecomplex* tmp_WORKDC	= NULL;

    double* R_Rprime	= NULL;
    double* invR_A_invRprime	= NULL;
    double* invU_invL_E	= NULL;

    doublecomplex* RC_RCprime			= NULL;
    doublecomplex* invRC_AC_invRCprime	= NULL;
    doublecomplex* invUC_invLC_EC		= NULL;

    int* IPVT	= NULL;

    IPARAM = (int*)malloc(11 * sizeof(int));
    memset(IPARAM, 0, 11 * sizeof(int));
    IPARAM[0] = 1;
    IPARAM[2] = (int) maxiter[0];
    IPARAM[6] = 1; // by default mode = 1

    IPNTR = (int*)malloc(14 * sizeof(int));
    memset(IPNTR, 0, 14 * sizeof(int));

    tmp_WORKD = (double*)malloc(N * sizeof(double));
    memset(tmp_WORKD, 0, N * sizeof(double));
    tmp_WORKDC = (doublecomplex*)malloc(N * sizeof(doublecomplex));
    memset(tmp_WORKDC, 0, N * sizeof(doublecomplex));

    // END VARIABLES

    // Info to compute eigenvalues and eigenvectors
    if (eigenvector != NULL)
    {
        RVEC = 1;
    }

    // MODE
    if (!strcmp(which, "SM") || (SIGMAR != 0 || SIGMAI != 0))
    {
        IPARAM[6] = 3;
        which = "LM";
    }

    // BMAT
    if ((matB == 0) || (IPARAM[6] == 1)) // if B = [] or mode = 1 -> bmat = 'I' : standart eigenvalue problem
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
            free(IPARAM);
            free(IPNTR);
            free(R);
            free(Rprime);
            free(RC);
            free(RCprime);
            free(tmp_WORKD);
            free(tmp_WORKDC);
            return -1;

        }
    }

    // NEV
    if ((!Acomplex && !Bcomplex && Asym == 1 && nev >= N) || (!(!Acomplex && !Bcomplex && Asym == 1) && nev >= N - 1))
    {
        free(IPARAM);
        free(IPNTR);
        free(R);
        free(Rprime);
        free(RC);
        free(RCprime);
        free(tmp_WORKD);
        free(tmp_WORKDC);
        return -2;
    }

    // B must be symmetric (hermitian) positive (semi-) positive
    if (matB != 0)
    {
        if (cholB[0]) // Comparison between B and upper triangular matrix
        {
            if (!Bcomplex) // B is real
            {
                for (i = 0 ; i < N ; i++)
                {
                    for (j = i + 1 ; j < N ; j++)
                    {
                        if (B[j + i * N] != 0)
                        {
                            free(IPARAM);
                            free(IPNTR);
                            free(R);
                            free(Rprime);
                            free(RC);
                            free(RCprime);
                            free(tmp_WORKD);
                            free(tmp_WORKDC);
                            free(IPVT);
                            return -3;
                        }
                    }
                }

                memcpy(Rprime, B, N * N * sizeof(double));

                // Compute the lower triangular matrix
                memset(R, 0, N * N * sizeof(double));
                for (i = 0 ; i < N ; i++)
                {
                    for (j = 0 ; j < N  ; j++)
                    {
                        R[i * N + j] = B[j * N + i];
                    }
                }
            }
            else	// if B is complex
            {
                for (i = 0 ; i < N ; i++)
                {
                    for (j = i + 1 ; j < N ; j++)
                    {
                        if (BC[j + i * N].r != 0 || BC[j + i * N].i != 0)
                        {
                            free(IPARAM);
                            free(IPNTR);
                            free(R);
                            free(Rprime);
                            free(RC);
                            free(RCprime);
                            free(tmp_WORKD);
                            free(tmp_WORKDC);
                            return -3;
                        }
                    }
                }

                memcpy(RCprime, BC, N * N * sizeof(doublecomplex));

                // Computes the lower triangular matrix BC
                memset(RC, 0, N * N * sizeof(doublecomplex));
                for (i = 0 ; i < N ; i++)
                {
                    for (j = 0 ; j < N ; j++)
                    {
                        RC[i * N + j].r = BC[j * N + i].r;
                        RC[i * N + j].i = (-1) * BC[j * N + i].i;
                    }
                }
            }

        }
    }

    if (!cholB[0] && IPARAM[6] == 1 && matB != 0)
    {
        if (!Bcomplex) // B is real
        {
            memcpy(R, B, N * N * sizeof(double));
            memcpy(Rprime, B, N * N * sizeof(double));

            C2F(dpotrf)("L", &N, R, &N, &INFO_CHOL); // Compute the lower triangular matrix R
            if (INFO_CHOL != 0) // Errors
            {
                free(IPARAM);
                free(IPNTR);
                free(R);
                free(Rprime);
                free(RC);
                free(RCprime);
                free(tmp_WORKD);
                free(tmp_WORKDC);
                return -3;
            }

            C2F(dpotrf)("U", &N, Rprime, &N, &INFO_CHOL);   // Compute the upper triangular matrix Rprime
            if (INFO_CHOL != 0)
            {
                free(IPARAM);
                free(IPNTR);
                free(R);
                free(Rprime);
                free(RC);
                free(RCprime);
                free(tmp_WORKD);
                free(tmp_WORKDC);
                return -3;
            }

            for (j = 0 ; j < N ; j++)
            {
                for (i = 0 ; i < j ; i++)
                {
                    R[i + j * N] = 0;
                    Rprime[j + i * N] = 0;
                }
            }
        }
        else	// B is complex
        {
            memcpy(RC, BC, N * N * sizeof(doublecomplex));
            memcpy(RCprime, BC, N * N * sizeof(doublecomplex));

            C2F(zpotrf)("L", &N, RC, &N, &INFO_CHOL); // Computes the lower triangular matrix
            if (INFO_CHOL != 0)
            {
                free(IPARAM);
                free(IPNTR);
                free(R);
                free(Rprime);
                free(RC);
                free(RCprime);
                free(tmp_WORKD);
                free(tmp_WORKDC);
                return -3;
            }

            C2F(zpotrf)("U", &N, RCprime, &N, &INFO_CHOL);	// Computes the upper triangular matrix
            if (INFO_CHOL != 0)
            {
                free(IPARAM);
                free(IPNTR);
                free(R);
                free(Rprime);
                free(RC);
                free(RCprime);
                free(tmp_WORKD);
                free(tmp_WORKDC);
                return -3;
            }

            for (j = 0 ; j < N ; j++)
            {
                for (i = 0 ; i < j ; i++)
                {
                    RC[i + j * N].r = 0;
                    RC[i + j * N].i = 0;

                    RCprime[j + i * N].r = 0;
                    RCprime[j + i * N].i = 0;
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

            // Compute LU decomposition AMSB = A - sigma*B
            if (matB == 0) // if B = [] -> standart eigenvalue problem : A - sigma
            {
                for (i = 0 ; i < N ; i++)
                {
                    AMSB[i + i * N] = AMSB[i + i * N] - SIGMAR;
                }
            }
            else	// generalized eigenvalue problem
            {
                if (cholB[0])
                {
                    if (R_Rprime == NULL)
                    {
                        R_Rprime = (double*)malloc(N * N * sizeof(double));
                        RtimesRprime(R_Rprime, R, Rprime, N);
                    }

                    for (i = 0 ; i < N * N ; i++)
                    {
                        AMSB[i] = AR[i] - (SIGMAR * R_Rprime[i]);
                    }
                }
                else
                {
                    for (i = 0 ; i < N * N ; i++)
                    {
                        AMSB[i] = AR[i] - (SIGMAR * B[i]);
                    }
                }
            }

            // LU decomposition
            IPVT = (int*) malloc(N * sizeof(int));
            memset(IPVT, 0, N * sizeof(int));
            C2F(dgetrf)(&N, &N, AMSB, &N, IPVT, &INFO_LU);

            // Computes the lower triangular matrix L
            L = (double*)malloc(N * N * sizeof(double));
            memset(L, 0, N * N * sizeof(double));

            for (i = 0 ; i < N ; i++)
            {
                for (j = 0 ; j < i ; j++)
                {
                    L[i + j * N] = AMSB[i + j * N];
                }

                L[i + i * N] = 1;
            }

            // Computes the upper triangular matrix U
            U = (double*)malloc(N * N * sizeof(double));
            memset(U, 0, N * N * sizeof(double));

            for (j = 0 ; j < N ; j++)
            {
                for (i = 0 ; i <= j ; i++)
                {
                    //if(i <= j)
                    U[i + j * N] = AMSB[i + j * N];
                }
            }

            // Computes the permutation matrix E
            E = (double*) malloc(N * N * sizeof(double));
            memset(E, 0, N * N * sizeof(double));

            for (i = 0 ; i < N ; i++)
            {
                E[i * N + i] = 1;
            }

            C2F(dlaswp)(&N, E, &N, &k1, &N, IPVT, &k1);

            free(AMSB);
            free(IPVT);
        }

        if (Asym) // DSAUPD
        {
            LWORKL = ncv * ncv + 8 * ncv;

            WORKL = (double*)malloc(LWORKL * sizeof(double));
            memset(WORKL, 0, LWORKL * sizeof(double));
        }
        else	// DNAUPD
        {
            LWORKL = 3 * ncv * (ncv + 2);

            WORKL = (double*)malloc(LWORKL * sizeof(double));
            memset(WORKL, 0, LWORKL * sizeof(double));
        }

        WORKD = (double*)malloc(3 * N * sizeof(double));
        memset(WORKD, 0, 3 * N * sizeof(double));

        V = (double*)malloc(N * ncv * sizeof(double));
        memset(V, 0, N * ncv * sizeof(double));

        while (IDO != 99)
        {
            if (Asym) // DSAUPD
            {
                C2F(dsaupd)(&IDO, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO[0]);
            }
            else	// DNAUPD
            {
                C2F(dnaupd)(&IDO, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO[0], 1L, 2L);
            }

            if (INFO[0] < 0)
            {
                free(IPARAM);
                free(IPNTR);
                free(R);
                free(Rprime);
                free(RC);
                free(RCprime);
                free(tmp_WORKD);
                free(tmp_WORKDC);
                free(WORKD);
                free(WORKL);
                free(V);
                free(U);
                free(L);
                free(E);
                if (R_Rprime != NULL)
                {
                    free(R_Rprime);
                }
                return -4;
            }

            if (IDO == -1 || IDO == 1 || IDO == 2)
            {
                if (IPARAM[6] == 1) // mode = 1
                {
                    if (matB == 0) // B = [] -> standart eigenvalue problem
                    {
                        // OP = A*x
                        C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, AR, &N, WORKD + IPNTR[0] - 1, &N, &beta, WORKD + IPNTR[1] - 1, &N);
                    }
                    else // generalized eigenvalue problem
                    {
                        // OP = inv(Rprime)*A*inv(R)*x
                        if (invR_A_invRprime == NULL)
                        {
                            invR_A_invRprime = (double*)malloc(N * N * sizeof(double));
                            invR_times_A_times_invRprime(invR_A_invRprime, R, AR, Rprime, N);
                        }

                        C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, invR_A_invRprime, &N, WORKD + IPNTR[0] - 1, &N, &beta, WORKD + IPNTR[1] - 1, &N);
                    }
                }
                else
                {
                    if (IPARAM[6] == 3) // mode = 3
                    {
                        if (matB == 0) // B = [] -> standart eigenvalue problem
                        {
                            if (IDO == 2)
                            {
                                // y = B*x where B = I so workd[ipntr[1]-1:ipntr[1]+N-1] = workd[ipntr[0]-1:ipntr[0]+N-1]
                                memcpy(WORKD + IPNTR[1] - 1, WORKD + IPNTR[0] - 1, N * sizeof(double));
                            }
                            else
                            {
                                // workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[0]-1:ipntr[0]+N-1]

                                if (invU_invL_E == NULL)
                                {
                                    invU_invL_E = (double*)malloc(N * N * sizeof(double));
                                    invU_times_invL_times_E(invU_invL_E, U, L, E, N);
                                }

                                C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, invU_invL_E, &N, WORKD + IPNTR[0] - 1, &N, &beta, WORKD + IPNTR[1] - 1, &N);
                            }
                        }
                        else  // matB == 1 so B is not empty and bmat = 'G'-> generalized eigenvalue problem
                        {
                            if (IDO == 2)
                            {
                                if (cholB[0]) // workd[ipntr[1]-1:ipntr[1]+N-1] = R * Rprime * workd[ipntr[0]-1:ipntr[0]+N-1]
                                {
                                    if (R_Rprime == NULL)
                                    {
                                        R_Rprime = (double*)malloc(N * N * sizeof(double));
                                        RtimesRprime(R_Rprime, R, Rprime, N);
                                    }

                                    C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, R_Rprime, &N, WORKD + IPNTR[0] - 1, &N, &beta, WORKD + IPNTR[1] - 1, &N);
                                }
                                else	//  workd[ipntr[1]-1:ipntr[1]+N-1] = B * workd[ipntr[0]-1:ipntr[0]+N-1]
                                {
                                    C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, B, &N, WORKD + IPNTR[0] - 1, &N, &beta, WORKD + IPNTR[1] - 1, &N);
                                }
                            }
                            else
                            {
                                if (IDO == -1)
                                {
                                    if (cholB[0])  // workd[ipntr[1]-1:ipntr[1]+N-1] = R * Rprime * workd[ipntr[0]-1:ipntr[0]+N-1]
                                    {
                                        if (R_Rprime == NULL)
                                        {
                                            R_Rprime = (double*)malloc(N * N * sizeof(double));
                                            RtimesRprime(R_Rprime, R, Rprime, N);
                                        }

                                        C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, R_Rprime, &N, WORKD + IPNTR[0] - 1, &N, &beta, WORKD + IPNTR[1] - 1, &N);
                                    }
                                    else	// workd[ipntr[1]-1:ipntr[1]+N-1] = B * workd[ipntr[0]-1:ipntr[0]+N-1]
                                    {
                                        C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, B, &N, WORKD + IPNTR[0] - 1, &N, &beta, WORKD + IPNTR[1] - 1, &N);
                                    }
                                    // compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[1]-1:ipntr[1]+N-1]

                                    if (invU_invL_E == NULL)
                                    {
                                        invU_invL_E = (double*)malloc(N * N * sizeof(double));
                                        invU_times_invL_times_E(invU_invL_E, U, L, E, N);
                                    }

                                    C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, invU_invL_E, &N, WORKD + IPNTR[1] - 1, &N, &beta, tmp_WORKD, &N);
                                    memcpy(WORKD + IPNTR[1] - 1, tmp_WORKD, N * sizeof(double));
                                }
                                else
                                {
                                    if (IDO == 1)
                                    {
                                        // computes workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[2]-1:ipntr[2]+N-1]
                                        if (invU_invL_E == NULL)
                                        {
                                            invU_invL_E = (double*)malloc(N * N * sizeof(double));
                                            invU_times_invL_times_E(invU_invL_E, U, L, E, N);
                                        }

                                        C2F(dgemm)("n", "n", &N, &iOne, &N, &alpha, invU_invL_E, &N, WORKD + IPNTR[2] - 1, &N, &beta, WORKD + IPNTR[1] - 1, &N);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        free(IPARAM);
                        free(IPNTR);
                        free(R);
                        free(Rprime);
                        free(RC);
                        free(RCprime);
                        free(tmp_WORKD);
                        free(tmp_WORKDC);
                        free(WORKD);
                        free(WORKL);
                        free(V);
                        free(U);
                        free(L);
                        free(E);

                        return -5;
                    }
                }
            }
        } // END WHILE

        free(L);
        free(U);
        free(E);

        if (R_Rprime != NULL)
        {
            free(R_Rprime);
        }

        if (invR_A_invRprime != NULL)
        {
            free(invR_A_invRprime);
        }

        if (invU_invL_E != NULL)
        {
            free(invU_invL_E);
        }

        SELECT = (int*)malloc(ncv * sizeof(int));
        memset(SELECT, 0, ncv * sizeof(int));

        if (Asym) // DSEUPD
        {
            D = (double*)malloc(nev * sizeof(double));
            memset(D, 0, nev * sizeof(double));

            Z = (double*)malloc(N * nev * sizeof(double));
            memset(Z, 0, N * nev * sizeof(double));

            C2F(dseupd)(&RVEC, HOWMNY, SELECT, D, Z, &LDV, &SIGMAR, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO_EUPD, 1L, 1L, 1L, 2L);

            if (INFO_EUPD != 0)
            {
                free(IPARAM);
                free(IPNTR);
                free(R);
                free(Rprime);
                free(RC);
                free(RCprime);
                free(tmp_WORKD);
                free(tmp_WORKDC);
                free(WORKD);
                free(WORKL);
                free(V);
                free(D);
                free(Z);
                free(SELECT);
                return -6;
            }
            else
            {
                for (i = 0 ; i < nev ; i++)
                {
                    eigenvalue[i].r = D[i];
                }

                if (RVEC)
                {
                    for (i = 0 ; i < N * nev ; i++)
                    {
                        eigenvector[i].r = Z[i];
                    }
                }
            }

            free(D);
            free(Z);
        }
        else	// DNEUPD
        {
            DR = (double*)malloc((nev + 1) * sizeof(double));
            memset(DR, 0, (nev + 1) * sizeof(double));

            DI = (double*)malloc((nev + 1) * sizeof(double));
            memset(DI, 0, (nev + 1) * sizeof(double));

            Z = (double*) malloc(N * (nev + 1) * sizeof(double));
            memset(Z, 0, N * (nev + 1) * sizeof(double));

            WORKEV = (double*)malloc(3 * ncv * sizeof(double));
            memset(WORKEV, 0, 3 * ncv * sizeof(double));

            C2F(dneupd)(&RVEC, HOWMNY, SELECT, DR, DI, Z, &LDV, &SIGMAR, &SIGMAI, WORKEV, bmat, &N, which, &nev, tol, RESID, &ncv, V, &LDV, IPARAM, IPNTR, WORKD, WORKL, &LWORKL, &INFO_EUPD);

            if (INFO_EUPD != 0)
            {
                free(IPARAM);
                free(IPNTR);
                free(R);
                free(Rprime);
                free(RC);
                free(RCprime);
                free(tmp_WORKD);
                free(tmp_WORKDC);
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
                for (i = 0 ; i < nev ; i++)
                {
                    eigenvalue[i].r = DR[i];
                    eigenvalue[i].i = DI[i];
                }

                if (RVEC)
                {
                    i = 0;
                    while (i <= (nev - 2))
                    {
                        for (j = 0; j < N; j++)
                        {
                            eigenvector[i * N + j].r = Z[i * N + j];
                            eigenvector[i * N + j].i = Z[(i + 1) * N + j];
                            eigenvector[(i + 1)*N + j].r = Z[i * N + j];
                            eigenvector[(i + 1)*N + j].i = -Z[(i + 1) * N + j];
                        }
                        i = i + 2;
                    }
                }

            }
            free(DR);
            free(DI);
            free(Z);
            free(WORKEV);
        }

        free(V);
        free(WORKD);
        free(WORKL);
        free(SELECT);
    }
    else // A or/and B complex
    {
        if (IPARAM[6] == 3)	// mode = 3
        {
            AMSBC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
            memcpy(AMSBC, AC, N * N * sizeof(doublecomplex));
            if (matB == 0)	// standart eigenvalue problem
            {
                for (i = 0 ; i < N ; i++)
                {
                    AMSBC[i + i * N].r = AMSBC[i + i * N].r - SIGMAR;
                    AMSBC[i + i * N].i = AMSBC[i + i * N].i - SIGMAI;
                }
            }
            else	// generalized eigenvalue problem
            {
                if (cholB[0])
                {
                    if (RC_RCprime == NULL)
                    {
                        RC_RCprime = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
                        RCtimesRCprime(RC_RCprime, RC, RCprime, N);
                    }

                    for (i = 0 ; i < N * N ; i++)
                    {
                        AMSBC[i].r = AMSBC[i].r - (SIGMAR * RC_RCprime[i].r + SIGMAI * RC_RCprime[i].i);
                        AMSBC[i].i = AMSBC[i].i - (SIGMAR * RC_RCprime[i].i + SIGMAI * RC_RCprime[i].r);
                    }
                }
                else
                {
                    for (i = 0 ; i < N * N ; i++)
                    {
                        AMSBC[i].r = AMSBC[i].r - (SIGMA[0].r * BC[i].r);
                        AMSBC[i].i = AMSBC[i].i - (SIGMA[0].i * BC[i].i);
                    }
                }
            }

            // LU decomposition
            IPVT = (int*) malloc(N * sizeof(int));
            memset(IPVT, 0, N * sizeof(int));
            C2F(zgetrf)(&N, &N, AMSBC, &N, IPVT, &INFO_LU);

            // Computes the lower triangular matrix L
            LC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
            memset(LC, 0, N * N * sizeof(doublecomplex));
            for (i = 0 ; i < N ; i++)
            {
                for (j = 0 ; j < i ; j++)
                {
                    LC[i + j * N].r = AMSBC[i + j * N].r;
                    LC[i + j * N].i = AMSBC[i + j * N].i;
                }
                LC[i + i * N].r = 1;
                LC[i + i * N].i = 0;
            }

            // Computes the upper triangular matrix U

            UC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
            memset(UC, 0, N * N * sizeof(doublecomplex));
            for (j = 0 ; j < N ; j++)
            {
                for (i = 0 ; i <= j ; i++)
                {
                    UC[i + j * N].r = AMSBC[i + j * N].r;
                    UC[i + j * N].i = AMSBC[i + j * N].i;
                }
            }

            // Computes the permutation matrix E
            EC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
            E = (double*)malloc(N * N * sizeof(double));
            memset(E, 0, N * N * sizeof(double));

            for (i = 0 ; i < N ; i++)
            {
                E[i * N + i] = 1;
            }

            C2F(dlaswp)(&N, E, &N, &k1, &N, IPVT, &k1);

            memset(EC, 0, N * N * sizeof(doublecomplex));
            for (i = 0 ; i < N * N ; i++)
            {
                EC[i].r = E[i];
            }

            free(AMSBC);
            free(IPVT);
        }

        LWORKL = 3 * ncv * ncv + 5 * ncv;

        VC = (doublecomplex*)malloc(N * ncv * sizeof(doublecomplex));
        memset(VC, 0, N * ncv * sizeof(doublecomplex));

        WORKLC = (doublecomplex*)malloc(LWORKL * sizeof(doublecomplex));
        memset(WORKLC, 0, LWORKL * sizeof(doublecomplex));

        WORKDC = (doublecomplex*)malloc(3 * N * sizeof(doublecomplex));
        memset(WORKDC, 0, 3 * N * sizeof(doublecomplex));

        RWORK = (double*)malloc(ncv * sizeof(double));
        memset(RWORK, 0, ncv * sizeof(double));

        while (IDO != 99)
        {
            C2F(znaupd)(&IDO, bmat, &N, which, &nev, tol, RESIDC, &ncv, VC, &LDV, IPARAM, IPNTR, WORKDC, WORKLC, &LWORKL, RWORK, &INFO[0]);

            if (INFO[0] < 0)
            {
                free(IPARAM);
                free(IPNTR);
                free(R);
                free(Rprime);
                free(RC);
                free(RCprime);
                free(tmp_WORKD);
                free(tmp_WORKDC);
                free(LC);
                free(UC);
                free(EC);
                free(E);
                free(WORKDC);
                free(WORKLC);
                free(VC);
                free(RWORK);
                if (RC_RCprime != NULL)
                {
                    free(RC_RCprime);
                }
                return -4;
            }

            if (IDO == -1 || IDO == 1 || IDO == 2)
            {
                if (IPARAM[6] == 1) // mode = 1
                {
                    if (matB == 0) // B = I
                    {
                        // OP = A*x
                        C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, AC, &N, WORKDC + IPNTR[0] - 1, &N, &betac, WORKDC + IPNTR[1] - 1, &N);
                    }
                    else
                    {
                        // OP = inv(R')*A*inv(R)*x
                        if (invRC_AC_invRCprime == NULL)
                        {
                            invRC_AC_invRCprime = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
                            invRC_times_AC_times_invRCprime(invRC_AC_invRCprime, RC, AC,  RCprime, N);
                        }

                        C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, invRC_AC_invRCprime, &N, WORKDC + IPNTR[0] - 1, &N, &betac, WORKDC + IPNTR[1] - 1, &N);
                    }
                }
                else
                {
                    if (IPARAM[6] == 3) // si mode = 3
                    {
                        if (matB == 0)	// B = [] -> matB is empty -> standart eigenvalue problem
                        {
                            if (IDO == 2)
                            {
                                // y = B*x where B = I so workd[ipntr[1]-1:ipntr[1]+N-1] = workd[ipntr[0]-1:ipntr[0]+N-1]
                                memcpy(WORKDC + IPNTR[1] - 1, WORKDC + IPNTR[0] - 1, N * sizeof(doublecomplex));
                            }
                            else
                            {
                                // workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[0]-1:ipntr[0]+N-1]
                                if (invUC_invLC_EC == NULL)
                                {
                                    invUC_invLC_EC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
                                    invUC_times_invLC_times_EC(invUC_invLC_EC, UC, LC, EC, N);
                                }
                                C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, invUC_invLC_EC, &N, WORKDC + IPNTR[0] - 1, &N, &betac, WORKDC + IPNTR[1] - 1, &N);
                            }

                        }
                        else  // matB == 1 so B is not empty and bmat = 'G'-> generalized eigenvalue problem
                        {
                            if (IDO == 2)
                            {
                                if (cholB[0]) // workd[ipntr[1]-1:ipntr[1]+N-1] = RC * RCprime * workd[ipntr[0]-1:ipntr[0]+N-1]
                                {
                                    if (RC_RCprime == NULL)
                                    {
                                        RC_RCprime = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
                                        RCtimesRCprime(RC_RCprime, RC, RCprime, N);
                                    }

                                    C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, RC_RCprime, &N, WORKDC + IPNTR[0] - 1, &N, &betac, WORKDC + IPNTR[1] - 1, &N);
                                }
                                else	// workd[ipntr[1]-1:ipntr[1]+N-1] = B *workd[ipntr[0]-1:ipntr[0]+N-1]
                                {
                                    C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, BC, &N, WORKDC + IPNTR[0] - 1, &N, &betac, WORKDC + IPNTR[1] - 1, &N);
                                }
                            }
                            else
                            {
                                if (IDO == -1)
                                {
                                    if (cholB[0])  // workd[ipntr[1]-1:ipntr[1]+N-1] = RC*RCprime*workd[ipntr[0]-1:ipntr[0]+N-1]
                                    {
                                        if (RC_RCprime == NULL)
                                        {
                                            RC_RCprime = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
                                            RCtimesRCprime(RC_RCprime, RC, RCprime, N);
                                        }

                                        C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, RC_RCprime, &N, WORKDC + IPNTR[0] - 1, &N, &betac, WORKDC + IPNTR[1] - 1, &N);
                                    }
                                    else	// workd[ipntr[1]-1:ipntr[1]+N-1] = B * workd[ipntr[0]-1:ipntr[0]+N-1]
                                    {
                                        C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, BC, &N, WORKDC + IPNTR[0] - 1, &N, &betac, WORKDC + IPNTR[1] - 1, &N);
                                    }

                                    // compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[1]-1:ipntr[1]+N-1]
                                    if (invUC_invLC_EC == NULL)
                                    {
                                        invUC_invLC_EC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
                                        invUC_times_invLC_times_EC(invUC_invLC_EC, UC, LC, EC, N);
                                    }

                                    C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, invUC_invLC_EC, &N, WORKDC + IPNTR[1] - 1, &N, &betac, tmp_WORKDC, &N);
                                    memcpy(WORKDC + IPNTR[1] - 1, tmp_WORKDC, N * sizeof(doublecomplex*));
                                }
                                else
                                {
                                    if (IDO == 1)
                                    {
                                        // compute workd[ipntr[1]-1:ipntr[1]+N-1] = inv(U)*inv(L)*inv(P)*workd[ipntr[2]-1:ipntr[2]+N-1]
                                        if (invUC_invLC_EC == NULL)
                                        {
                                            invUC_invLC_EC = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
                                            invUC_times_invLC_times_EC(invUC_invLC_EC, UC, LC, EC, N);
                                        }

                                        C2F(zgemm)("n", "n", &N, &iOne, &N, &alphac, invUC_invLC_EC, &N, WORKDC + IPNTR[2] - 1, &N, &betac, WORKDC + IPNTR[1] - 1, &N);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        free(IPARAM);
                        free(IPNTR);
                        free(R);
                        free(Rprime);
                        free(RC);
                        free(RCprime);
                        free(tmp_WORKD);
                        free(tmp_WORKDC);
                        free(LC);
                        free(UC);
                        free(EC);
                        free(E);
                        free(WORKDC);
                        free(WORKLC);
                        free(VC);
                        free(RWORK);
                        if (RC_RCprime != NULL)
                        {
                            free(RC_RCprime);
                        }

                        if (invRC_AC_invRCprime != NULL)
                        {
                            free(invRC_AC_invRCprime);
                        }

                        if (invUC_invLC_EC != NULL)
                        {
                            free(invUC_invLC_EC);
                        }
                        return -5;
                    }
                }
            }
        } // END WHILE
        free(LC);
        free(UC);
        free(EC);
        free(E);

        if (RC_RCprime != NULL)
        {
            free(RC_RCprime);
        }

        if (invRC_AC_invRCprime != NULL)
        {
            free(invRC_AC_invRCprime);
        }

        if (invUC_invLC_EC != NULL)
        {
            free(invUC_invLC_EC);
        }

        SELECT = (int*)malloc(ncv * sizeof(int));
        memset(SELECT, 0, ncv * sizeof(int));

        DC = (doublecomplex*)malloc((nev + 1) * sizeof(doublecomplex));
        memset(DC, 0, (nev + 1) * sizeof(doublecomplex));

        ZC = (doublecomplex*)malloc(N * nev * sizeof(doublecomplex));
        memset(ZC, 0, N * nev * sizeof(doublecomplex));

        WORKEVC = (doublecomplex*)malloc(2 * ncv * sizeof(doublecomplex));
        memset(WORKEVC, 0, 2 * ncv * sizeof(doublecomplex));

        C2F(zneupd)(&RVEC, HOWMNY, SELECT, DC, ZC, &LDV, SIGMA, WORKEVC, bmat, &N, which, &nev, tol, RESIDC, &ncv, VC, &LDV, IPARAM, IPNTR, WORKDC, WORKLC, &LWORKL, RWORK, &INFO_EUPD);
        if (INFO_EUPD != 0)
        {
            free(IPARAM);
            free(IPNTR);
            free(R);
            free(Rprime);
            free(RC);
            free(RCprime);
            free(tmp_WORKD);
            free(tmp_WORKDC);
            free(WORKDC);
            free(WORKLC);
            free(VC);
            free(SELECT);
            free(DC);
            free(ZC);
            free(WORKEVC);
            free(RWORK);

            return -6;
        }
        else
        {
            if (!RVEC)
            {
                for (i = 0; i < nev; i++)
                {
                    eigenvalue[i].r = DC[i].r;
                    eigenvalue[i].i = DC[i].i;
                }
            }
            else  // return eigenvalues and eigenvectors
            {
                memcpy(eigenvalue, DC, nev * sizeof(doublecomplex));
                memcpy(eigenvector, ZC, N * nev * sizeof(doublecomplex));
            }
        }

        free(SELECT);
        free(DC);
        free(ZC);
        free(WORKEVC);

        free(VC);
        free(tmp_WORKDC);
        free(tmp_WORKD);
        free(WORKDC);
        free(WORKLC);
        free(RWORK);
    }

    free(IPARAM);
    free(IPNTR);

    free(R);
    free(Rprime);
    free(RC);
    free(RCprime);

    return 0;
}

