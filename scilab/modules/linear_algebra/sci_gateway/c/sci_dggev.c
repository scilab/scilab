/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Michaël Baudin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "stack-c.h"
#include "elementary_functions.h"
#include "vfinite.h"
#include "do_error_number.h"
#include "MALLOC.h"
#include "msgs.h"
#include "assembleEigenvectors.h"
#include "gw_linear_algebra.h"

extern int C2F(dsyev) ();
extern int C2F(dlaset) ();
extern int C2F(dcopy) ();
extern int C2F(dggev) ();

//
// intdggev --
//   Interface to LAPACK's dggev
//   Computes the generalized eigenvalues and, if required, the generalized
//   eigenvectors of two real matrix.
//   Possible uses :
//   * With 1 LHS :
//       eigenvalues=spec(A,B)
//     where
//       A : square real matrix of size NxN
//       B : square real matrix of size NxN
//       eigenvalues : matrix of size Nx1 with right eigenvectors, type complex
//   * With 2 LHS :
//       [alpha,beta]=spec(A,B)
//     where
//       alpha,beta : the coefficients such that the generalized eigenvalues
//         are alpha(i)/beta(i), where alpha is of type complex, beta is real
//   * With 3 LHS :
//       [alpha,beta,R]=spec(A,B)
//     where
//       R : the matrix of generalized right eigenvectors, type complex
//   * With 4 LHS :
//       [alpha,beta,L,R]=spec(A,B)
//     where
//       L : the matrix of generalized left eigenvectors, type complex
//
int sci_dggev(char *fname, unsigned long fname_len)
{
    int totalsize;
    int iRowsA = 0;
    int iColsA = 0;
    int iRowsB = 0;
    int iColsB = 0;
    int ONE = 1;
    int iWorkSize;
    int INFO;

    double *pdblWork = NULL;
    char JOBVR;
    char JOBVL;

    double *pdblMatrixA = NULL;
    double *pdblMatrixB = NULL;
    double *pdblFinalAlphaReal = NULL;  //SCILAB return Var
    double *pdblFinalAlphaImg = NULL;   //SCILAB return Var
    double *pdblFinalBeta = NULL;   //SCILAB return Var
    double *pdblFinalRReal = NULL;  //SCILAB return Var
    double *pdblFinalRImg = NULL;   //SCILAB return Var
    double *pdblFinalLReal = NULL;  //SCILAB return Var
    double *pdblFinalLImg = NULL;   //SCILAB return Var

    CheckRhs(2, 2);
    CheckLhs(1, 4);

    GetRhsVarMatrixDouble(1, &iRowsA, &iColsA, &pdblMatrixA);
    GetRhsVarMatrixDouble(2, &iRowsB, &iColsB, &pdblMatrixB);

    totalsize = iRowsA * iColsA;

    if (iRowsA != iColsA)
    {
        Err = 1;
        SciError(20);
        return 0;
    }
    if (iRowsB != iColsB)
    {
        Err = 2;
        SciError(20);
        return 0;
    }
    if (iRowsA != iRowsB)
    {
        SciError(267);
        return 0;
    }

    if (iColsA == 0)
    {
        if (Lhs == 1)
        {
            LhsVar(1) = 1;
            return 0;
        }
        else if (Lhs == 2)
        {
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            return 0;
        }
        else if (Lhs == 3)
        {
            int ZERO = 0;
            int lVR;

            CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &ZERO, &ZERO, &lVR);
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            LhsVar(3) = 3;
            return 0;
        }
        else if (Lhs == 4)
        {
            int ZERO = 0;
            int lVR;
            int lVL;

            CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &ZERO, &ZERO, &lVR);
            CreateVar(4, MATRIX_OF_DOUBLE_DATATYPE, &ZERO, &ZERO, &lVL);
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            LhsVar(3) = 3;
            LhsVar(4) = 4;
            return 0;
        }
    }

    if (C2F(vfinite) (&totalsize, pdblMatrixA) == 0)
    {
        SciError(264);
        return 0;
    }
    if (C2F(vfinite) (&totalsize, pdblMatrixB) == 0)
    {
        SciError(264);
        return 0;
    }

    iAllocComplexMatrixOfDouble(3, iColsA, ONE, &pdblFinalAlphaReal, &pdblFinalAlphaImg);
    iAllocMatrixOfDouble(4, iColsA, ONE, &pdblFinalBeta);
    if (Lhs == 3 || Lhs == 4)
    {
        iAllocComplexMatrixOfDouble(5, iColsA, iColsA, &pdblFinalRReal, &pdblFinalRImg);
    }
    if (Lhs == 4)
    {
        iAllocComplexMatrixOfDouble(6, iColsA, iColsA, &pdblFinalLReal, &pdblFinalLImg);
    }

    iWorkSize = Max(1, 8 * iColsA);
    pdblWork = (double *)MALLOC(iWorkSize * sizeof(double));

    if (Lhs == 1 || Lhs == 2)
    {
        JOBVL = 'N';            // Do not compute left eigenvectors
        JOBVR = 'N';            // Do not compute right eigenvectors
    }
    else if (Lhs == 3)
    {
        JOBVL = 'N';            // Do not compute left eigenvectors
        JOBVR = 'V';            // Compute right eigenvectors
    }
    else if (Lhs == 4)
    {
        JOBVL = 'V';            // Compute left eigenvectors
        JOBVR = 'V';            // Compute right eigenvectors
    }
    C2F(dggev) (&JOBVL, &JOBVR, &iColsA, pdblMatrixA, &iColsA, pdblMatrixB, &iColsA, pdblFinalAlphaReal,
                pdblFinalAlphaImg, pdblFinalBeta, pdblFinalLReal, &iColsA, pdblFinalRReal, &iColsA, pdblWork, &iWorkSize, &INFO);
    //     SUBROUTINE DGGEV( JOBVL, JOBVR, N, A, LDA, B, LDB, ALPHAR,
    //     $     ALPHAI, BETA, VL, LDVL, VR, LDVR, WORK, LWORK, INFO )
    FREE(pdblWork);
    if (INFO != 0)
    {
        if (INFO <= iColsA)
        {
            C2F(msgs) (&ONE, &INFO);
        }
        else
        {
            SciError(24);
            return 0;
        }
    }
    if (Lhs == 1)
    {
        int i;

        for (i = 0; i < iColsA; i++)
        {
            pdblFinalAlphaReal[i] = pdblFinalAlphaReal[i] / pdblFinalBeta[i];
            pdblFinalAlphaImg[i] = pdblFinalAlphaImg[i] / pdblFinalBeta[i];
        }
        LhsVar(1) = 3;
    }
    else if (Lhs == 2)
    {
        LhsVar(1) = 3;
        LhsVar(2) = 4;
    }
    if (Lhs == 3 || Lhs == 4)
    {
        assembleEigenvectorsInPlace(iRowsA, pdblFinalAlphaImg, pdblFinalRReal, pdblFinalRImg);
    }
    if (Lhs == 3)
    {
        LhsVar(1) = 3;
        LhsVar(2) = 4;
        LhsVar(3) = 5;
    }
    if (Lhs == 4)
    {
        assembleEigenvectorsInPlace(iRowsA, pdblFinalAlphaImg, pdblFinalLReal, pdblFinalLImg);
        LhsVar(1) = 3;
        LhsVar(2) = 4;
        LhsVar(3) = 6;
        LhsVar(4) = 5;
    }
    return 0;
}
