/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __COMMON_H__
#define __COMMON_H__

#include "core_math.h"

#include "abs.h"
#include "cos.h"
#include "exp.h"
#include "log.h"
#include "pythag.h"
#include "sin.h"
#include "tan.h"
#include "sqrt.h"
#include "sign.h"

/*
'E' or 'e',   DLAMCH := eps ( relative machine precision )
'S' or 's ,   DLAMCH := sfmin ( safe minimum, such that 1/sfmin does not overflow )
'B' or 'b',   DLAMCH := base ( base of the machine )
'P' or 'p',   DLAMCH := eps*base ( eps*base )
'N' or 'n',   DLAMCH := t ( number of (base) digits in the mantissa )
'R' or 'r',   DLAMCH := rnd ( 1.0 when rounding occurs in addition, 0.0 otherwis )
'M' or 'm',   DLAMCH := emin ( minimum exponent before (gradual) underflow )
'U' or 'u',   DLAMCH := rmin ( underflow threshold - base**(emin-1) )
'L' or 'l',   DLAMCH := emax ( largest exponent before overflow )
'O' or 'o',   DLAMCH := rmax ( overflow threshold  - (base**emax)*(1-eps) )
*/

#include <stdlib.h>
#include "machine.h"

#include "doublecomplex.h"
#include "numericconstants_interface.h"

#ifdef __cplusplus
#define isRealZero(x)						(fabs(static_cast<double>(x)) <= nc_eps())
#define ZeroIsZero(x)						(fabs(static_cast<double>(x)) <= nc_eps() ? 0 : static_cast<double>(x))
#else
#define isZero(x)							(fabs((double)x) <= nc_eps())
#define ZeroIsZero(x)						(fabs((double)x) <= nc_eps() ? 0 : (double)x)
#endif

#define getUnderflowThreshold()				nc_double_min()
#define getOverflowThreshold()				nc_double_max()
#define isEqual(x,y)						fabs((double)x - (double)y) <= nc_eps()

extern double C2F(dlamch) (const char *_pszCommand, unsigned long int);
extern double C2F(logp1) (double *_pdblVal);
extern int C2F(dgemm) (char *_pstTransA, char *_pstTransB, int *_piN, int *_piM, int *_piK, double *_pdblAlpha, double *_pdblA, int *_piLdA,
                       double *_pdblB, int *_piLdB, double *_pdblBeta, double *_pdblC, int *_piLdC);
extern int C2F(zgemm) (char *_pstTransA, char *_pstTransB, int *_piN, int *_piM, int *_piK, double *_pdblAlpha, double *_pdblA, int *_piLdA,
                       double *_pdblB, int *_piLdB, double *_pdblBeta, double *_pdblC, int *_piLdC);
extern int C2F(dswap) (int *_piSize, double *_pdblX, int *_piIncX, double *_pdblY, int *_piIncY);
extern double C2F(dasum) (int *_iSize, double *_pdbl, int *_iInc);
extern int C2F(dcopy) (int *_iSize, double *_pdblSrc, int *_piIncSrc, double *_pdblDest, int *_piDest);
extern int C2F(dscal) (int *_iSize, double *_pdblVal, double *_pdblDest, int *_iInc);
extern int C2F(zscal) (int *_iSize, doublecomplex * _pdblVal, doublecomplex * _pdblDest, int *_iInc);
extern int C2F(dset) (int *_iSize, double *_pdblVal, double *_pdblDest, int *_iInc);
extern double C2F(dlange) (char *_pstNorm, int *_piM, int *_piN, double *_pdblA, int *_piLdA, double *_pdblWork);
extern int C2F(dlacpy) (char *_pstUplo, int *piM, int *_piN, double *_pdblA, int *_piLdA, double *_pdblB, int *_piLdB);
extern int C2F(dgecon) (char *_pstNORM, int *_piN, double *_pdblA, int *_piLDA, double *_pdblANORM, double *_pdblRCOND, double *_pdblWORK,
                        int *_piIWORK, int *_piINFO);
extern int C2F(dgelsy1) (int *_piM, int *_piN, int *_piNRHS, double *_pdblA, int *_piLDA, double *_pdblB, int *_piLDB, int *_piJPVT,
                         double *_pdblRCOND, int *_piRANK, double *_pdblWORK, int *_piLWORK, int *_piINFO);
extern double C2F(zlange) (char *_pstNORM, int *_piM, int *_piN, double *_pdblA, int *_piLDA, double *_pdblWORK);
extern int C2F(zlacpy) (char *_pstUPLO, int *_piM, int *_piN, double *_pdblA, int *_piLDA, double *_pdblB, int *_piLDB);
extern void C2F(zgecon) (char *_pstNORM, int *_piN, doublecomplex * _pdblA, int *_piLDA, double *_pdblANORM, double *_pdblRNORM,
                         doublecomplex * _pdblWORK, double *_pdblRWORD, int *_piINFO);
extern int C2F(zgelsy1) (int *_piM, int *_piN, int *_piNRHS, doublecomplex * pdblA, int *_piLDA, doublecomplex * _pdblB, int *_piLDB, int *_piJPVT,
                         double *_pdblRCOND, int *_piRANK, doublecomplex * _pdblWORK, int *_piLWORK, double *_pdblRWORK, int *_piINFO);
extern double C2F(ddot) (int *_ipSize, double *_pdblVal1, int *_piInc1, double *_pdblVal2, int *_piInc2);
extern void C2F(wmul) (double *ar, double *ai, double *br, double *bi, double *cr, double *ci);
extern void C2F(wdiv) (double *ar, double *ai, double *br, double *bi, double *cr, double *ci);
extern void C2F(dad) (double *a, int *na, int *i1, int *i2, int *j1, int *j2, double *r, int *isw);
extern int C2F(entier) (int *_iSize, double *_pdbl, int *_pi);
extern int C2F(simple) (int *_iSize, double *_pdbl, float *_pf);
extern double C2F(nearfloat) (double*, double*);
extern int C2F(daxpy)(int* N, double* DA, double* DX, int* INCX, double* DY, int* INCY);
extern int C2F(zaxpy)(int* N, doublecomplex* ZA, doublecomplex* ZX, int* INCX, doublecomplex* ZY, int* INCY);
extern int C2F(dsymv)(char* UPLO, int* N, double* ALPHA, double* A, int* LDA, double* X, int* INCX, double* BETA, double* Y, int* INCY);

// dgemv performs matrix-vector operations
extern int C2F(dgemv) (char* trans, int* m, int* n, double* alpha, double* A, int* lda,
                       double* x, int* incx, double* beta, double* y, int* incy);
extern int C2F(zgemv) (char* trans, int* m, int* n, doublecomplex* alpha, doublecomplex* A,
                       int* lda, doublecomplex* x, int* incx, doublecomplex* beta, doublecomplex* y, int* incy);

// dgetrf computes an LU factorization of a general M by N matrix A (double) using partial pivoting with row interchanges
extern int C2F(dgetrf)(int* m, int* n, double* A, int* lda, int* ipiv, int* info);

// dgetrs solves a linear system using the factors computed by dgetrf
extern int C2F(dgetrs) (char* trans, int* n, int* nrhs, double* A, int *lda, int* ipiv, double* B, int* ldb, int* info);

// dpotrf computes the cholesky factorization of a real symmetric positive definite matrix A
extern int C2F(dpotrf)(char* uplo, int* n, double* A, int* lda, int* info);

// zpotrf computes the cholesky factorization of a real hermitian positive definite matrix A
extern int C2F(zpotrf)(char* uplo, int* n, doublecomplex* A, int* lda, int* info);

// dtrsm solves a triangular linear system
extern int C2F(dtrsm) (char* side, char* uplo, char* trans, char* diag, int* m, int* n, double* alpha, double* A, int* lda, double* B, int* ldb);
// ztrsm solve a triangular linear system
extern int C2F(ztrsm) (char* side, char* uplo, char* trans, char* diag, int* m, int* n, doublecomplex* alpha, doublecomplex* A, int* lda, doublecomplex* B, int* ldb);
// dsyrk does a rank k symmetric update
extern int C2F(dsyrk) (char* uplo, char* trans, int* n, int* k, double* alpha,
                       double* A, int* lda, double* beta, double* B, int* ldb);
// ztrmm multiply by a triangular matrix
extern int C2F(ztrmm) (char* side, char* uplo, char* trans, char* diag, int* m, int* n, doublecomplex* alphac,
                       doublecomplex* A, int* lda, doublecomplex* B, int* ldb);
// ztrmv multiply a vector by a triangular matrix
extern int C2F(ztrmv) (char* uplo, char* trans, char* diag, int* n, doublecomplex* A, int* lda, doublecomplex* x, int* incx);
// dtrmv multiply a vector by a triangular matrix
extern int C2F(dtrmv) (char* uplo, char* trans, char* diag, int* n, double* A, int* lda, double* x, int* incx);
// zgetrs solves a linear system using the factors computed by zgetrf
extern int C2F(zgetrs) (char *_pstTRANS, int *_piN, int *_piNRHS, doublecomplex *_pdblA, int *_piLDA, int *_piIPIV, doublecomplex *_pdblB, int *_piLDB,
                        int *_piINFO);
// zgetrf computes an LU factorization of a general M by N matrix A (complex*16) using partial pivoting with row interchanges
extern int C2F(zgetrf) (int *_piM, int *_piN, doublecomplex *_pdblA, int *_piLDA, int *_piIPIV, int *_piINFO);

#endif /* __COMMON_H__ */
