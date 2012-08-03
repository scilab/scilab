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
#include "msgs.h"
#include "vfinite.h"
#include "assembleEigenvectors.h"
#include "gw_linear_algebra.h"

extern int C2F(dsyev) ();
extern int C2F(dlaset) ();
extern int C2F(dcopy) ();
extern int C2F(zggev) ();
extern int C2F(wwdiv) (double *ar, double *ai, double *br, double *bi, double *cr, double *ci, int *ierr);
extern int C2F(dggev) ();

static int isarrayzero(int size, double *values);
static int intzggev_real(char *fname);
static int intzggev_complex(char *fname);

//
// intzggev --
//   Interface to LAPACK's zggev
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
int sci_zggev(char *fname, unsigned long fname_len)
{
    int totalsizeA;
    int totalsizeB;
    int iRowsA = 0;
    int iColsA = 0;
    int iRowsB = 0;
    int iColsB = 0;
    int isImaginaryMatrixAZero;
    int isImaginaryMatrixBZero;

    double *pdblMatrixAReal = NULL;
    double *pdblMatrixAImg = NULL;
    double *pdblMatrixBReal = NULL;
    double *pdblMatrixBImg = NULL;

    CheckRhs(2, 2);

    GetRhsVarMatrixComplex(1, &iRowsA, &iColsA, &pdblMatrixAReal, &pdblMatrixAImg);
    totalsizeA = iRowsA * iColsA;
    GetRhsVarMatrixComplex(2, &iRowsB, &iColsB, &pdblMatrixBReal, &pdblMatrixBImg);
    totalsizeB = iRowsB * iColsB;

    // Check the imaginary parts of both matrices.
    isImaginaryMatrixAZero = isarrayzero(totalsizeA, pdblMatrixAImg);
    isImaginaryMatrixBZero = isarrayzero(totalsizeB, pdblMatrixBImg);
    if (isImaginaryMatrixAZero == 1 && isImaginaryMatrixBZero == 1)
    {
        // If both imaginary parts are zero, then the used solver is DGGEV.
        // This is a work-around for bug #3652
        // http://bugzilla.scilab.org/show_bug.cgi?id=3652
        // When LAPACK bug is solved, this thin layer may be removed.
        // For simplicity reasons, the two branches are separated rather than factored.
        // Moreover, when the bug is solved (if ever ?), the current layer will be easier to disconnect.
        intzggev_real(fname);
    }
    else
    {
        // If one imaginary part non-zero, then the used solver is ZGGEV.
        intzggev_complex(fname);
    }
    return 0;
}

//
// isarrayzero --
//   Returns 1 if the given array is zero.
// Arguments
//   size : the size of the array
//   values : the values
//
static int isarrayzero(int size, double *values)
{
    int index;
    int result;

    result = 1;
    for (index = 0; index < size; index++)
    {
        if (values[index] != 0)
        {
            result = 0;
            break;
        }
    }
    return result;
}

//
// intzggev_complex --
//   Perform the interface in the case where A and B are general complex
//   matrices.
//   Interface to LAPACK's zggev
//
static int intzggev_complex(char *fname)
{
    int totalsize;
    int iRowsA = 0;
    int iColsA = 0;
    int iRowsB = 0;
    int iColsB = 0;
    int ONE = 1;
    int iWorkSize;
    int INFO;
    int iRWorkSize;

    char JOBVR;
    char JOBVL;

    double *pdblMatrixAReal = NULL;
    double *pdblMatrixAImg = NULL;
    double *pdblMatrixBReal = NULL;
    double *pdblMatrixBImg = NULL;
    double *pdblRWork = NULL;   // Used by LAPACK
    double *pdblFinalAlphaReal = NULL;  //SCILAB return Var
    double *pdblFinalAlphaImg = NULL;   //SCILAB return Var
    double *pdblFinalBetaReal = NULL;   //SCILAB return Var, in complex case
    double *pdblFinalBetaImg = NULL;    //SCILAB return Var, in complex case
    double *pdblFinalRReal = NULL;  //SCILAB return Var
    double *pdblFinalRImg = NULL;   //SCILAB return Var
    double *pdblFinalLReal = NULL;  //SCILAB return Var
    double *pdblFinalLImg = NULL;   //SCILAB return Var
    doublecomplex *pdblWork = NULL;
    doublecomplex *pdblMatrixA = NULL;
    doublecomplex *pdblMatrixB = NULL;
    doublecomplex *pdblLeftvectors = NULL;  // Used by LAPACK
    doublecomplex *pdblRightvectors = NULL; // Used by LAPACK
    doublecomplex *pdblAlpha = NULL;    //Used by LAPACK
    doublecomplex *pdblBeta = NULL; //Used by LAPACK

    CheckRhs(2, 2);
    CheckLhs(1, 4);

    GetRhsVarMatrixComplex(1, &iRowsA, &iColsA, &pdblMatrixAReal, &pdblMatrixAImg);
    totalsize = iRowsA * iColsA;
    pdblMatrixA = oGetDoubleComplexFromPointer(pdblMatrixAReal, pdblMatrixAImg, totalsize);
    GetRhsVarMatrixComplex(2, &iRowsB, &iColsB, &pdblMatrixBReal, &pdblMatrixBImg);
    pdblMatrixB = oGetDoubleComplexFromPointer(pdblMatrixBReal, pdblMatrixBImg, totalsize);

    if (iRowsA != iColsA)
    {
        Err = 1;
        SciError(20);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }
    if (iRowsB != iColsB)
    {
        Err = 2;
        SciError(20);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }
    if (iRowsA != iRowsB)
    {
        SciError(267);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }

    if (iColsA == 0)
    {
        if (Lhs == 1)
        {
            LhsVar(1) = 1;
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
            return 0;
        }
        else if (Lhs == 2)
        {
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
            return 0;
        }
        else if (Lhs == 3)
        {
            int ZERO = 0;
            int lVR;

            CreateVar(3, MATRIX_OF_COMPLEX_DATATYPE, &ZERO, &ZERO, &lVR);
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            LhsVar(3) = 3;
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
            return 0;
        }
        else if (Lhs == 4)
        {
            int ZERO = 0;
            int lVR;
            int lVL;

            CreateVar(3, MATRIX_OF_COMPLEX_DATATYPE, &ZERO, &ZERO, &lVR);
            CreateVar(4, MATRIX_OF_COMPLEX_DATATYPE, &ZERO, &ZERO, &lVL);
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            LhsVar(3) = 3;
            LhsVar(4) = 4;
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
            return 0;
        }
    }

    if (C2F(vfiniteComplex) (&totalsize, pdblMatrixA) == 0)
    {
        SciError(264);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }
    if (C2F(vfiniteComplex) (&totalsize, pdblMatrixB) == 0)
    {
        SciError(264);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }
    iAllocComplexMatrixOfDouble(3, iColsA, ONE, &pdblFinalAlphaReal, &pdblFinalAlphaImg);
    iAllocComplexMatrixOfDouble(4, iColsA, ONE, &pdblFinalBetaReal, &pdblFinalBetaImg);
    pdblAlpha = (doublecomplex *) MALLOC(iColsA * sizeof(doublecomplex));
    pdblBeta = (doublecomplex *) MALLOC(iColsA * sizeof(doublecomplex));

    if (Lhs == 3 || Lhs == 4)
    {
        iAllocComplexMatrixOfDouble(5, iColsA, iColsA, &pdblFinalRReal, &pdblFinalRImg);
        pdblRightvectors = (doublecomplex *) MALLOC(totalsize * sizeof(doublecomplex));
    }
    if (Lhs == 4)
    {
        iAllocComplexMatrixOfDouble(6, iColsA, iColsA, &pdblFinalLReal, &pdblFinalLImg);
        pdblLeftvectors = (doublecomplex *) MALLOC(totalsize * sizeof(doublecomplex));
    }

    iWorkSize = Max(1, 2 * iColsA);
    pdblWork = (doublecomplex *) MALLOC(iWorkSize * sizeof(doublecomplex));
    iRWorkSize = Max(1, 8 * iColsA);
    pdblRWork = (double *)MALLOC(sizeof(double) * iRWorkSize);

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
    C2F(zggev) (&JOBVL, &JOBVR, &iColsA, pdblMatrixA, &iColsA, pdblMatrixB, &iColsA, pdblAlpha,
                pdblBeta, pdblLeftvectors, &iColsA, pdblRightvectors, &iColsA, pdblWork, &iWorkSize, pdblRWork, &INFO);
    //     ZGGEV( JOBVL, JOBVR, N, A, LDA, B, LDB, ALPHA, BETA,
    //     $                  VL, LDVL, VR, LDVR, WORK, LWORK, RWORK, INFO )

    FREE(pdblWork);
    FREE(pdblRWork);
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
        /*int i;
         * int ierr;
         * double ar, ai, br, bi, cr, ci;
         * for(i = 0 ; i < iColsA ; i++)
         * {
         * ar = pdblAlpha[i].r;
         * ai = pdblAlpha[i].i;
         * br = pdblBeta[i].r;
         * bi = pdblBeta[i].i;
         * C2F(wwdiv)(&ar, &ai, &br, &bi, &cr, &ci, &ierr);
         * pdblFinalAlphaReal[i]=cr;
         * pdblFinalAlphaImg[i]=ci;
         * } */
        double *ar, *ai, *br, *bi, *rr, *ri;
        int ia = 2, ib = 2, ir = 1, ierr;

        LhsVar(1) = 3;
        ar = &pdblAlpha[0].r;
        ai = &pdblAlpha[0].i;
        br = &pdblBeta[0].r;
        bi = &pdblBeta[0].i;
        rr = pdblFinalAlphaReal;
        ri = pdblFinalAlphaImg;
        C2F(wwrdiv) (ar, ai, &ia, br, bi, &ib, rr, ri, &ir, &iColsA, &ierr);
    }
    if (Lhs == 2 || Lhs == 3 || Lhs == 4)
    {
        // Transfert eigenvalues from doublecomplex to real and imaginary parts
        vGetPointerFromDoubleComplex(pdblAlpha, iColsA, pdblFinalAlphaReal, pdblFinalAlphaImg);
        vGetPointerFromDoubleComplex(pdblBeta, iColsA, pdblFinalBetaReal, pdblFinalBetaImg);
    }
    if (Lhs == 2)
    {
        LhsVar(1) = 3;
        LhsVar(2) = 4;
    }
    if (Lhs == 3 || Lhs == 4)
    {
        // Transfert eigenvectors from doublecomplex to real and imaginary parts
        vGetPointerFromDoubleComplex(pdblRightvectors, totalsize, pdblFinalRReal, pdblFinalRImg);
    }
    if (Lhs == 3)
    {
        LhsVar(1) = 3;
        LhsVar(2) = 4;
        LhsVar(3) = 5;
    }
    if (Lhs == 4)
    {
        // Transfert eigenvectors from doublecomplex to real and imaginary parts
        vGetPointerFromDoubleComplex(pdblLeftvectors, totalsize, pdblFinalLReal, pdblFinalLImg);
        LhsVar(1) = 3;
        LhsVar(2) = 4;
        LhsVar(3) = 6;
        LhsVar(4) = 5;
    }
    FREE(pdblAlpha);
    FREE(pdblBeta);
    if (Lhs == 3 || Lhs == 4)
    {
        FREE(pdblRightvectors);
    }
    if (Lhs == 4)
    {
        FREE(pdblLeftvectors);
    }
    vFreeDoubleComplexFromPointer(pdblMatrixA);
    vFreeDoubleComplexFromPointer(pdblMatrixB);
    return 0;
}

//
// intzggev_real --
//   Perform the interface in the case where A and B are complex
//   matrices where the imaginary parts are ZERO.
//   Interface to LAPACK's dggev
//   The following source code is an adaptation of intdggev.
//
static int intzggev_real(char *fname)
{
    int totalsize;
    int iRowsA = 0;
    int iColsA = 0;
    int iRowsB = 0;
    int iColsB = 0;
    int ONE = 1;
    int iWorkSize = 0;
    int INFO = 0;

    char JOBVR;
    char JOBVL;

    double *pdblMatrixAReal = NULL;
    double *pdblMatrixAImg = NULL;
    double *pdblMatrixBReal = NULL;
    double *pdblMatrixBImg = NULL;
    double *pdblFinalAlphaReal = NULL;  //SCILAB return Var
    double *pdblFinalAlphaImg = NULL;   //SCILAB return Var
    double *pdblFinalBeta = NULL;   //SCILAB return Var, in real-only case
    double *pdblFinalRReal = NULL;  //SCILAB return Var
    double *pdblFinalRImg = NULL;   //SCILAB return Var
    double *pdblFinalLReal = NULL;  //SCILAB return Var
    double *pdblFinalLImg = NULL;   //SCILAB return Var
    double *pdblWork = NULL;
    doublecomplex *pdblMatrixA = NULL;
    doublecomplex *pdblMatrixB = NULL;
    doublecomplex *pdblLeftvectors = NULL;  // Used by LAPACK
    doublecomplex *pdblRightvectors = NULL; // Used by LAPACK

    CheckRhs(2, 2);
    CheckLhs(1, 4);

    GetRhsVarMatrixComplex(1, &iRowsA, &iColsA, &pdblMatrixAReal, &pdblMatrixAImg);
    totalsize = iRowsA * iColsA;
    pdblMatrixA = oGetDoubleComplexFromPointer(pdblMatrixAReal, pdblMatrixAImg, totalsize);
    GetRhsVarMatrixComplex(2, &iRowsB, &iColsB, &pdblMatrixBReal, &pdblMatrixBImg);
    pdblMatrixB = oGetDoubleComplexFromPointer(pdblMatrixBReal, pdblMatrixBImg, totalsize);

    if (iRowsA != iColsA)
    {
        Err = 1;
        SciError(20);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }
    if (iRowsB != iColsB)
    {
        Err = 2;
        SciError(20);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }
    if (iRowsA != iRowsB)
    {
        SciError(267);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }

    if (iColsA == 0)
    {
        if (Lhs == 1)
        {
            LhsVar(1) = 1;
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
            return 0;
        }
        else if (Lhs == 2)
        {
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
            return 0;
        }
        else if (Lhs == 3)
        {
            int ZERO = 0;
            int lVR;

            CreateVar(3, MATRIX_OF_COMPLEX_DATATYPE, &ZERO, &ZERO, &lVR);
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            LhsVar(3) = 3;
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
            return 0;
        }
        else if (Lhs == 4)
        {
            int ZERO = 0;
            int lVR;
            int lVL;

            CreateVar(3, MATRIX_OF_COMPLEX_DATATYPE, &ZERO, &ZERO, &lVR);
            CreateVar(4, MATRIX_OF_COMPLEX_DATATYPE, &ZERO, &ZERO, &lVL);
            LhsVar(1) = 1;
            LhsVar(2) = 2;
            LhsVar(3) = 3;
            LhsVar(4) = 4;
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
            return 0;
        }
    }

    if (C2F(vfiniteComplex) (&totalsize, pdblMatrixA) == 0)
    {
        SciError(264);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
        return 0;
    }
    if (C2F(vfiniteComplex) (&totalsize, pdblMatrixB) == 0)
    {
        SciError(264);
        vFreeDoubleComplexFromPointer(pdblMatrixA);
        vFreeDoubleComplexFromPointer(pdblMatrixB);
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
    C2F(dggev) (&JOBVL, &JOBVR, &iColsA, pdblMatrixAReal, &iColsA, pdblMatrixBReal, &iColsA, pdblFinalAlphaReal,
                pdblFinalAlphaImg, pdblFinalBeta, pdblFinalLReal, &iColsA, pdblFinalRReal, &iColsA, pdblWork, &iWorkSize, &INFO);
    //     SUBROUTINE DGGEV( JOBVL, JOBVR, N, A, LDA, B, LDB, ALPHAR,
    //     $     ALPHAI, BETA, VL, LDVL, VR, LDVR, WORK, LWORK, INFO )
    if (pdblWork)
    {
        FREE(pdblWork);
        pdblWork = NULL;
    }
    if (INFO != 0)
    {
        if (INFO <= iColsA)
        {
            C2F(msgs) (&ONE, &INFO);
        }
        else
        {
            SciError(24);
            vFreeDoubleComplexFromPointer(pdblMatrixA);
            vFreeDoubleComplexFromPointer(pdblMatrixB);
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
    if (Lhs == 2)
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
    if (Lhs == 3 || Lhs == 4)
    {
        FREE(pdblRightvectors);
    }
    if (Lhs == 4)
    {
        FREE(pdblLeftvectors);
    }
    vFreeDoubleComplexFromPointer(pdblMatrixA);
    vFreeDoubleComplexFromPointer(pdblMatrixB);
    return 0;
}
