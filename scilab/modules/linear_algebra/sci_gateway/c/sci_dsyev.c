/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Michaël Baudin
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "stack-c.h" 
#include "elementary_functions.h" 
#include "do_error_number.h"
#include "MALLOC.h"
#include "assembleEigenvectors.h"
#include "gw_linear_algebra.h"

extern int C2F(dsyev)();
extern int C2F(dlaset)();
extern int C2F(dcopy)();

//
// intdsyev --
//   Interface to LAPACK's DSYEV
//   Computes the eigenvalues and, if required, the eigenvectors of a symetric real matrix.
//   Possible uses :
//   * With 1 LHS :
//       eigenvalues=spec(A)
//     where 
//       A : symetric, square, real matrix of size NxN
//       eigenvalues : matrix of size Nx1, type real
//   * With 2 LHS :
//       [eigenvectors,eigenvalues]=spec(A)
//     where 
//       eigenvalues : matrix of size NxN with eigenvalues as diagonal terms, type real
//       eigenvectors : matrix of size NxN, type real
//
int sci_dsyev(char *fname, unsigned long fname_len)
{	
	int totalsize;

	int iRows = 0;
	int iCols = 0;
	int ONE = 1;
	int INFO;
	int iWorkSize;

	char UPLO;
	char JOBZ;

	double *pdblRealData	= NULL;
	double *pdblWork = NULL;
	double *pdblFinalEigenvalues = NULL; //SCILAB return Var
	double *pdblEigenValues	= NULL; //return by LAPACK

	CheckRhs(1,1) ;
	CheckLhs(1,2) ;

	GetRhsVarMatrixDouble(1, &iRows, &iCols, &pdblRealData);
	totalsize = iRows * iCols;
	
	if (iRows!=iCols)
	{
		SciError(20);
		return 0;
	}
	if (iCols==0)
	{
		if (Lhs==1)
		{
			LhsVar(1) = 1;
			return 0;
		}
		else if (Lhs==2)
		{
			int lV;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&iCols,&iCols,&lV);
			LhsVar(1) = 2;
			LhsVar(2) = 1;
			return 0;
		}
	}
	if (C2F(vfinite)(&totalsize,pdblRealData)==0)
	{
		SciError(264);
		return 0;
	}
	if (Lhs==1)
	{
		iAllocMatrixOfDouble(2, iCols, ONE, &pdblFinalEigenvalues);
	}
	else
	{
		iAllocMatrixOfDouble(2, iCols, iCols, &pdblFinalEigenvalues);
	}

	iAllocMatrixOfDouble(3, iCols, ONE, &pdblEigenValues);

	iWorkSize = Max(1,3*iCols - 1);
	pdblWork = (double*)MALLOC(iWorkSize * sizeof(double));

	if (Lhs==1)
	{
		JOBZ = 'N'; // Compute eigenvalues only;
	}
	else
	{
		JOBZ = 'V'; // Compute eigenvalues and eigenvectors.
	}
	UPLO = 'U';
	C2F(dsyev)( &JOBZ, &UPLO, &iCols, pdblRealData, &iCols, pdblEigenValues,
	     pdblWork, &iWorkSize, &INFO );
	//     SUBROUTINE DSYEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, INFO )
	FREE(pdblWork);
	if (INFO!=0)
	{
		SciError(24);
	}
	if(Lhs==1)
	{
		C2F(dcopy)(&iCols, pdblEigenValues, &ONE, pdblFinalEigenvalues, &ONE);
		LhsVar(1)=2;
	}
	else
	{
		assembleEigenvaluesFromDoublePointer(iRows, pdblEigenValues, pdblFinalEigenvalues);
		LhsVar(1)=1; // Eigenvectors are stored in matrix A, which is variable #1
		LhsVar(2)=2; // Eigenvalues are stored in variable #2
	}
	return 0;
}

