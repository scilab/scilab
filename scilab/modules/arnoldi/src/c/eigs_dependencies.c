/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Adeline CARNIS
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "eigs_dependencies.h"

// dgemm performs one of the matrix-matrix operations
extern int C2F(dgemm)(char* transa, char* transb, int* m, int* n, int* k, double* alpha, double* A, int* lda, double* B, int* ldb, double* beta, double* C, int* ldc); 

// dgetrf computes an LU factorization of a general M by N matrix A (double) using partial pivoting with row interchanges
extern int C2F(dgetrf)(int* m, int* n, double* A, int* lda, int* ipiv, int* info); 

// dgetri computes the inverse of a matrix using the LU factorization computed by dgetrf
extern int C2F(dgetri)(int* n, double* A, int* lda, int* ipiv, double* work, int* lworkl, int* info); 

// zgemm performs one of the matrix-matrix operations
extern int C2F(zgemm)(char* transa, char* transb, int* m, int* n, int* k, doublecomplex* alpha, doublecomplex* A, int* lda,
	doublecomplex* B, int* ldb, doublecomplex* beta, doublecomplex* C, int* ldc);

// zgetrd computes an LU factorization of a general M by N matrix A (complex*16) using partial pivoting with row interchanges
extern int C2F(zgetrf)(int* m, int* n, doublecomplex* A, int* lda, int* ipiv, int* info); 

// zgetri computes the inverse of a matrix using the LU factorization computed by zgetrf
extern int C2F(zgetri)(int* n, doublecomplex* A, int* lda, int* ipiv, doublecomplex* work, int* lworkl, int* info);

static double alpha = 1.;
static double beta = 0.;

static doublecomplex alphac = {1.,0.};
static doublecomplex betac = {0.,0.};

void RtimesRprime(double* result, double* R, double* Rprime, int N)
{
	C2F(dgemm)("n", "n", &N, &N, &N, &alpha, R, &N, Rprime, &N, &beta, result, &N);
}

void invR_times_A_times_invRprime(double* result, double* R, double* A, double* Rprime, int N)
{
	double* invRxA = NULL;
	int* IPVT = NULL;
	double* work = NULL;
	double* invR = NULL;
	double* invRprime = NULL;
	int INFO_LU = 0;

	invRxA = (double*)malloc(N * N * sizeof(double));
	invR = (double*)malloc(N * N * sizeof(double));
	invRprime = (double*)malloc(N * N * sizeof(double));
	work = (double*)malloc(N * N * sizeof(double));


	IPVT = (int*) malloc(N * sizeof(int));
	memset(IPVT, 0, N * sizeof(int));

	// inv(R)
	memcpy(invR, R, N * N * sizeof(double));   // copy R to invR
	C2F(dgetrf)(&N, &N, invR ,&N, IPVT, &INFO_LU);	// LU decomposition

	memset(work, 0, N * N * sizeof(double));
	C2F(dgetri)(&N, invR, &N, IPVT, work, &N, &INFO_LU);  // Obtain inverse matrix R

	// inv(Rprime)
	memset(IPVT, 0, N * sizeof(int));
	memcpy(invRprime, Rprime, N * N * sizeof(double));	
	C2F(dgetrf)(&N, &N, invRprime, &N, IPVT, &INFO_LU);	// LU decomposition

	memset(work, 0, N * N * sizeof(double));
	C2F(dgetri)(&N, invRprime, &N, IPVT, work, &N,&INFO_LU);	// Obtain inverse matrix Rprime

	C2F(dgemm)("n", "n", &N, &N, &N, &alpha, invR, &N, A, &N, &beta, invRxA, &N);

	C2F(dgemm)("n", "n", &N, &N, &N, &alpha, invRxA, &N, invRprime, &N, &beta, result, &N);

	free(invRxA);
	free(IPVT);
	free(work);
	free(invR);
	free(invRprime);
}

void invU_times_invL_times_E(double* result, double* U, double* L, double* E, int N)
{
	double* invUxinvL = NULL;
	int* IPVT = NULL;
	double* work = NULL;
	double* invU = NULL;
	double* invL = NULL;
	int INFO_LU = 0;

	invUxinvL = (double*)malloc(N * N * sizeof(double));
	invU = (double*)malloc(N * N * sizeof(double));
	invL = (double*)malloc(N * N* sizeof(double));
	work = (double*)malloc(N * N * sizeof(double));

	IPVT = (int*) malloc(N * sizeof(int));
	memset(IPVT, 0, N * sizeof(int));
	
	// inv L -> L obtained with LU decomposition
	memcpy(invL, L, N * N * sizeof(double));
	C2F(dgetrf)(&N, &N, invL, &N, IPVT, &INFO_LU); // LU decomposition

	memset(work, 0, N * N * sizeof(double));
	C2F(dgetri)(&N, invL, &N, IPVT, work, &N, &INFO_LU);  // inv(L)

	// inv U -> U obtained with LU decomposition
	memcpy(invU, U, N * N * sizeof(double));
	memset(IPVT, 0, N*sizeof(int));
	C2F(dgetrf)(&N, &N, invU, &N, IPVT, &INFO_LU); // LU decomposition

	memset(work, 0, N * N * sizeof(double));
	C2F(dgetri)(&N, invU, &N, IPVT, work, &N, &INFO_LU); // inv(U)

	C2F(dgemm)("n", "n", &N, &N, &N, &alpha, invU, &N, invL, &N, &beta, invUxinvL, &N);

	C2F(dgemm)("n", "n", &N, &N, &N, &alpha, invUxinvL, &N, E, &N, &beta, result, &N);

	free(invUxinvL);
	free(IPVT);
	free(work);
	free(invU);
	free(invL);
}


void RCtimesRCprime(doublecomplex* result, doublecomplex* R, doublecomplex* Rprime, int N)
{
	C2F(zgemm)("n", "n", &N, &N, &N, &alphac, R, &N, Rprime, &N, &betac, result, &N);
}

void invRC_times_AC_times_invRCprime(doublecomplex* result, doublecomplex* R, doublecomplex* A, doublecomplex* Rprime, int N)
{
	doublecomplex* invRxA = NULL;
	int* IPVT = NULL;
	doublecomplex* work = NULL;
	doublecomplex* invR = NULL;
	doublecomplex* invRprime = NULL;
	int INFO_LU = 0;

	invRxA = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
	invR = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
	invRprime = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
	work = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));


	IPVT = (int*) malloc(N * sizeof(int));
	memset(IPVT, 0, N * sizeof(int));

	// inv(R)
	memcpy(invR, R, N * N * sizeof(doublecomplex));   // copy R to invR
	C2F(zgetrf)(&N, &N, invR ,&N, IPVT, &INFO_LU);	// LU decomposition

	memset(work, 0, N * N * sizeof(doublecomplex));
	C2F(zgetri)(&N, invR, &N, IPVT, work, &N, &INFO_LU);  // Obtain inverse matrix R

	// inv(Rprime)
	memset(IPVT, 0, N * sizeof(int));
	memcpy(invRprime, Rprime, N * N * sizeof(doublecomplex));	
	C2F(zgetrf)(&N, &N, invRprime, &N, IPVT, &INFO_LU);	// LU decomposition

	memset(work, 0, N * N * sizeof(doublecomplex));
	C2F(zgetri)(&N, invRprime, &N, IPVT, work, &N,&INFO_LU);	// Obtain inverse matrix Rprime

	C2F(zgemm)("n", "n", &N, &N, &N, &alphac, invR, &N, A, &N, &betac, invRxA, &N);

	C2F(zgemm)("n", "n", &N, &N, &N, &alphac, invRxA, &N, invRprime, &N, &betac, result, &N);

	free(invRxA);
	free(IPVT);
	free(work);
	free(invR);
	free(invRprime);
}


void invUC_times_invLC_times_EC(doublecomplex* result, doublecomplex* U, doublecomplex* L, doublecomplex* E, int N)
{
	doublecomplex* invUxinvL = NULL;
	int* IPVT = NULL;
	doublecomplex* work = NULL;
	doublecomplex* invU = NULL;
	doublecomplex* invL = NULL;
	int INFO_LU = 0;

	invUxinvL = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
	invU = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
	invL = (doublecomplex*)malloc(N * N* sizeof(doublecomplex));
	work = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));

	IPVT = (int*) malloc(N * sizeof(int));
	memset(IPVT, 0, N * sizeof(int));
	
	// inv L -> L obtained with LU decomposition
	memcpy(invL, L, N * N * sizeof(doublecomplex));
	C2F(zgetrf)(&N, &N, invL, &N, IPVT, &INFO_LU); // LU decomposition

	memset(work, 0, N * N * sizeof(doublecomplex));
	C2F(zgetri)(&N, invL, &N, IPVT, work, &N, &INFO_LU);  // inv(L)

	// inv U -> U obtained with LU decomposition
	memcpy(invU, U, N * N * sizeof(doublecomplex));
	memset(IPVT, 0, N*sizeof(int));
	C2F(zgetrf)(&N, &N, invU, &N, IPVT, &INFO_LU); // LU decomposition

	memset(work, 0, N * N * sizeof(doublecomplex));
	C2F(zgetri)(&N, invU, &N, IPVT, work, &N, &INFO_LU); // inv(U)

	C2F(zgemm)("n", "n", &N, &N, &N, &alphac, invU, &N, invL, &N, &betac, invUxinvL, &N);

	C2F(zgemm)("n", "n", &N, &N, &N, &alphac, invUxinvL, &N, E, &N, &betac, result, &N);

	free(invUxinvL);
	free(IPVT);
	free(work);
	free(invU);
	free(invL);
}