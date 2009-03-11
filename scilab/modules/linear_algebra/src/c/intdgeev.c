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
extern int C2F(dsyev)();
extern int C2F(dlaset)();
extern int C2F(dcopy)();
extern int C2F(dgeev)();


//
// intdgeev --
//   Interface to LAPACK's DGEEV
//   Computes the eigenvalues and, if required, the eigenvectors of a un-symetric real matrix.
//   Possible uses :
//   * With 1 LHS :
//       eigenvalues=spec(A)
//     where
//       A : un-symetric, square real matrix of size NxN
//       eigenvalues : matrix of size Nx1 with right eigenvectors, type complex
//   * With 2 LHS :
//       [eigenvectors,eigenvalues]=spec(A)
//     where 
//       eigenvalues : matrix of size NxN with eigenvalues as diagonal terms
//       eigenvectors : matrix of size NxN with right eigenvectors, type complex
//
int intdgeev(char *fname)
{	
	int totalsize;
	int iRows = 0;
	int iCols = 0;
	int ONE = 1;
	int ZERO = 0;
	int iWorkSize;

	int INFO;

	char JOBVR;
	char JOBVL;

	double dlbZero = 0;

	double *pdblWork = NULL;
	double *pdblRealData	= NULL;
	double *pdblFinalEigenvaluesReal = NULL; //SCILAB return Var
	double *pdblFinalEigenvaluesImg = NULL; //SCILAB return Var
	double *pdblFinalEigenvectorsReal = NULL; //SCILAB return Var
	double *pdblFinalEigenvectorsImg = NULL; //SCILAB return Var
	double *pdblEigenvalReal	= NULL; //return by LAPACK
	double *pdblEigenvalImg	= NULL; //return by LAPACK
	double *pdblRightvectors	= NULL; //return by LAPACK
	double *pdblLeftvectors	= NULL; //return by LAPACK

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
			int lD;
			int ZERO = 0;
			CreateVar(2,MATRIX_OF_COMPLEX_DATATYPE,&ZERO,&ZERO,&lD);
			LhsVar(1) = 2;
			return 0;
		}
		else if (Lhs==2)
		{
			int lD;
			int ZERO = 0;
			int lV;
			CreateVar(2,MATRIX_OF_COMPLEX_DATATYPE,&ZERO,&ZERO,&lD);
			CreateVar(3,MATRIX_OF_COMPLEX_DATATYPE,&ZERO,&ZERO,&lV);

			LhsVar(1) = 2;
			LhsVar(2) = 3;

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
		iAllocMatrixOfDoubleComplex(2, iCols, ONE, &pdblFinalEigenvaluesReal, &pdblFinalEigenvaluesImg);
	}
	else
	{
		iAllocMatrixOfDoubleComplex(2, iCols, iCols, &pdblFinalEigenvaluesReal, &pdblFinalEigenvaluesImg);
		iAllocMatrixOfDoubleComplex(3, iCols, iCols, &pdblFinalEigenvectorsReal, &pdblFinalEigenvectorsImg);
		pdblRightvectors = (double*)MALLOC(totalsize * sizeof(double));
	}
	pdblEigenvalReal = (double*)MALLOC(iCols * sizeof(double));
	pdblEigenvalImg = (double*)MALLOC(iCols * sizeof(double));

	if (Lhs==2)
	{
		iWorkSize = 4*iCols;
	}
	else
	{
		iWorkSize = 3*iCols;
	}
	pdblWork = (double*)MALLOC(iWorkSize * sizeof(double));


    JOBVL = 'N'; // Never compute left eigenvectors
	if (Lhs==1)
	{
		JOBVR = 'N'; // Do not compute right eigenvectors
	}
	else
	{
		JOBVR = 'V'; // Compute right eigenvectors.
	}
	C2F(dgeev)( &JOBVL, &JOBVR, &iCols, pdblRealData, &iCols, pdblEigenvalReal, pdblEigenvalImg,
        pdblLeftvectors, &iCols, pdblRightvectors, &iCols, pdblWork, &iWorkSize, &INFO );
//     SUBROUTINE DGEEV( JOBVL, JOBVR, N, A, LDA, WR, WI, VL, LDVL, VR, LDVR, 
//     $     WORK, INFO )
	FREE(pdblWork);
	if (INFO!=0)
	{
		SciError(24);
		return 0;
	}
	if(Lhs==1)
	{
		// Fill eigenvalues with computed real and imaginary parts
		int INCX = 1;
		int INCY = 1;
		C2F(dcopy)(&iCols,pdblEigenvalReal,&INCX,pdblFinalEigenvaluesReal,&INCY);
		C2F(dcopy)(&iCols,pdblEigenvalImg,&INCX,pdblFinalEigenvaluesImg,&INCY);
// DCOPY(N,DX,INCX,DY,INCY)
	} else {
		assembleComplexEigenvaluesFromDoublePointer(iRows, pdblEigenvalReal, pdblEigenvalImg, 
			pdblFinalEigenvaluesReal, pdblFinalEigenvaluesImg);

		assembleEigenvectorsSourceToTarget(iRows, pdblEigenvalImg, 
			pdblRightvectors, 
			pdblFinalEigenvectorsReal, pdblFinalEigenvectorsImg);

	}
      
	if (Lhs==1)
	{
         LhsVar(1) = 2;
	}
	else
	{

         LhsVar(1) = 3;
         LhsVar(2) = 2;
		 FREE(pdblRightvectors);
	}
	FREE(pdblEigenvalReal);
	FREE(pdblEigenvalImg);
	return 0;
}
