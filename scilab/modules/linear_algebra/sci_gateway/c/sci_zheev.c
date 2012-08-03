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
#include "vfinite.h"
#include "assembleEigenvectors.h"
#include "gw_linear_algebra.h"

extern int C2F(dlaset) ();
extern int C2F(zheev) ();

//
// intzheev --
//   Interface to LAPACK's ZHEEV
//   Computes the eigenvalues and, if required, the eigenvectors of a complex symmetric matrix.
//   Possible uses :
//   * With 1 LHS :
//       eigenvalues=spec(A)
//     where 
//       A : symmetric, square matrix of size NxN
//       eigenvalues : matrix of size Nx1, type real
//   * With 2 LHS :
//       [eigenvectors,eigenvalues]=spec(A)
//     where 
//       A : square matrix of size NxN
//       eigenvalues : matrix of size NxN with eigenvalues as diagonal terms, type real
//       eigenvectors : matrix of size NxN, type complex
//
int sci_zheev(char *fname, unsigned long fname_len)
{
    int totalsize;
    int iRows = 0;
    int iCols = 0;
    int ONE = 1;
    int iWorkSize;
    int iRWorkSize;
    int INFO;

    char JOBZ;
    char UPLO;

    double *pdblDataReal = NULL;
    double *pdblDataImg = NULL;
    double *pdblFinalEigenvalues = NULL;    //SCILAB return Var
    double *pdblEigenValues = NULL; //return by LAPACK
    double *pdblRWork = NULL;   // Used by LAPACK
    double *pdblFinalEigenvectorsReal;  // returned by Scilab
    double *pdblFinalEigenvectorsImg;   // returned by Scilab
    doublecomplex *pdblData = NULL;
    doublecomplex *pdblWork = NULL; // Used by LAPACK

    CheckRhs(1, 1);
    CheckLhs(1, 2);

    GetRhsVarMatrixComplex(1, &iRows, &iCols, &pdblDataReal, &pdblDataImg);
    totalsize = iRows * iCols;
    pdblData = oGetDoubleComplexFromPointer(pdblDataReal, pdblDataImg, totalsize);

    if (iRows != iCols)
    {
        Err = 1;
        SciError(20);
        vFreeDoubleComplexFromPointer(pdblData);
        return 0;
    }
    if (iCols == 0)
    {
        if (Lhs == 1)
        {
            LhsVar(1) = 1;
            vFreeDoubleComplexFromPointer(pdblData);
            return 0;
        }
        else if (Lhs == 2)
        {
            int lD;

            CreateVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iCols, &iCols, &lD);
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            vFreeDoubleComplexFromPointer(pdblData);
            return 0;
        }
    }
    if (C2F(vfiniteComplex) (&totalsize, pdblData) == 0)
    {
        SciError(264);
        vFreeDoubleComplexFromPointer(pdblData);
        return 0;
    }
    if (Lhs == 1)
    {
        iAllocMatrixOfDouble(2, iCols, ONE, &pdblFinalEigenvalues);
    }
    else
    {
        iAllocMatrixOfDouble(2, iCols, iCols, &pdblFinalEigenvalues);
        iAllocComplexMatrixOfDouble(3, iCols, iCols, &pdblFinalEigenvectorsReal, &pdblFinalEigenvectorsImg);
    }

    pdblEigenValues = (double *)MALLOC(sizeof(double) * iCols);

    iWorkSize = Max(1, 2 * iCols - 1);
    pdblWork = (doublecomplex *) MALLOC(sizeof(doublecomplex) * iWorkSize);
    iRWorkSize = Max(1, 3 * iCols - 2);
    pdblRWork = (double *)MALLOC(sizeof(double) * iRWorkSize);

    if (Lhs == 1)
    {
        JOBZ = 'N';             // Compute eigenvalues only;
    }
    else
    {
        JOBZ = 'V';             // Compute eigenvalues and eigenvectors.
    }
    UPLO = 'U';
    C2F(zheev) (&JOBZ, &UPLO, &iCols, pdblData, &iCols, pdblEigenValues, pdblWork, &iWorkSize, pdblRWork, &INFO);
//      SUBROUTINE ZHEEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, RWORK,
//     $                  INFO )
    FREE(pdblWork);
    FREE(pdblRWork);
    if (INFO != 0)
    {
        SciError(24);
    }
    if (Lhs == 1)
    {
        int INCX = 1;
        int INCY = 1;

        C2F(dcopy) (&iCols, pdblEigenValues, &INCX, pdblFinalEigenvalues, &INCY);
        LhsVar(1) = 2;
    }
    else
    {
        assembleEigenvaluesFromDoublePointer(iRows, pdblEigenValues, pdblFinalEigenvalues);
        vGetPointerFromDoubleComplex(pdblData, totalsize, pdblFinalEigenvectorsReal, pdblFinalEigenvectorsImg);
        LhsVar(1) = 3;          // Eigenvectors are stored in variable #3
        LhsVar(2) = 2;          // Eigenvalues are stored in variable #2
    }
    FREE(pdblEigenValues);
    vFreeDoubleComplexFromPointer(pdblData);
    return 0;
}
