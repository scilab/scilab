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
extern int C2F(zgeev) ();

//
// intzgeev --
//   Interface to LAPACK's ZGEEV
//   Computes the eigenvalues and, if required, the eigenvectors of a complex asymmetric matrix.
//   Possible uses :
//   * With 1 LHS :
//     eigenvalues=spec(A)
//   where 
//     A : symmetric, square matrix of size NxN
//     eigenvalues : matrix of size Nx1, type complex
//   * With 2 LHS :
//     [eigenvectors,eigenvalues]=spec(A)
//   where 
//     A : square matrix of size NxN
//     eigenvalues : matrix of size NxN with eigenvalues as diagonal terms, type complex
//     eigenvectors : matrix of size NxN, type complex
//
int sci_zgeev(char *fname, unsigned long fname_len)
{

    int totalsize;
    int iRows = 0;
    int iCols = 0;
    int ONE = 1;
    int iWorkSize;
    int INFO;

    char JOBVL;
    char JOBVR;

    double *pdblDataReal = NULL;
    double *pdblDataImg = NULL;
    double *pdblFinalEigenvaluesReal = NULL;    //SCILAB return Var
    double *pdblFinalEigenvaluesImg = NULL; //SCILAB return Var
    double *pdblFinalEigenvectorsReal = NULL;   //SCILAB return Var
    double *pdblFinalEigenvectorsImg = NULL;    //SCILAB return Var
    doublecomplex *pdblData = NULL;
    doublecomplex *pdblEigenValues = NULL;  //return by LAPACK
    doublecomplex *pdblWork = NULL; // Used by LAPACK
    doublecomplex *pdblRWork = NULL;    // Used by LAPACK
    doublecomplex *pdblLeftvectors = NULL;  // Used by LAPACK
    doublecomplex *pdblRightvectors = NULL; // Used by LAPACK

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
            int lD;

            CreateVar(2, MATRIX_OF_COMPLEX_DATATYPE, &iCols, &iCols, &lD);
            LhsVar(1) = 2;
            vFreeDoubleComplexFromPointer(pdblData);
            return 0;
        }
        else if (Lhs == 2)
        {
            int lD;
            int lV;

            CreateVar(2, MATRIX_OF_COMPLEX_DATATYPE, &iCols, &iCols, &lD);
            CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &iCols, &iCols, &lV);
            LhsVar(1) = 2;
            LhsVar(2) = 3;
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
        iAllocComplexMatrixOfDouble(2, iCols, ONE, &pdblFinalEigenvaluesReal, &pdblFinalEigenvaluesImg);
    }
    else
    {
        iAllocComplexMatrixOfDouble(2, iCols, iCols, &pdblFinalEigenvaluesReal, &pdblFinalEigenvaluesImg);
        iAllocComplexMatrixOfDouble(3, iCols, iCols, &pdblFinalEigenvectorsReal, &pdblFinalEigenvectorsImg);
        pdblRightvectors = (doublecomplex *) MALLOC(sizeof(doublecomplex) * totalsize);
    }

    pdblEigenValues = (doublecomplex *) MALLOC(sizeof(doublecomplex) * iCols);

    iWorkSize = Max(1, 2 * iCols);
    pdblWork = (doublecomplex *) MALLOC(sizeof(doublecomplex) * iWorkSize);
    pdblRWork = (doublecomplex *) MALLOC(sizeof(doublecomplex) * 2 * iCols);

    JOBVL = 'N';
    if (Lhs == 1)
    {
        JOBVR = 'N';            // Compute eigenvalues only;
    }
    else
    {
        JOBVR = 'V';            // Compute eigenvalues and eigenvectors.
    }
    C2F(zgeev) (&JOBVL, &JOBVR, &iCols, pdblData, &iCols, pdblEigenValues,
                pdblLeftvectors, &iCols, pdblRightvectors, &iCols, pdblWork, &iWorkSize, pdblRWork, &INFO);
//     SUBROUTINE ZGEEV( JOBVL, JOBVR, N, A, LDA, W, VL, LDVL,
//     $     VR, LDVR, WORK, LWORK, RWORK, INFO )
    FREE(pdblWork);
    FREE(pdblRWork);
    if (INFO != 0)
    {
        SciError(24);
    }
    if (Lhs == 2)
    {
        // Transfert eigenvalues
        assembleComplexEigenvaluesFromDoubleComplexPointer(iRows, pdblEigenValues, pdblFinalEigenvaluesReal, pdblFinalEigenvaluesImg);
        // Transfert eigenvectors from doublecomplex to real and imaginary parts
        vGetPointerFromDoubleComplex(pdblRightvectors, totalsize, pdblFinalEigenvectorsReal, pdblFinalEigenvectorsImg);
    }
    else
    {
        // Transfert eigenvalues from doublecomplex to real and imaginary parts
        vGetPointerFromDoubleComplex(pdblEigenValues, iCols, pdblFinalEigenvaluesReal, pdblFinalEigenvaluesImg);
    }
    if (Lhs == 1)
    {
        LhsVar(1) = 2;
    }
    else
    {
        LhsVar(1) = 3;
        LhsVar(2) = 2;
    }
    FREE(pdblEigenValues);
    if (Lhs == 2)
    {
        FREE(pdblRightvectors);
    }
    vFreeDoubleComplexFromPointer(pdblData);
    return 0;
}
