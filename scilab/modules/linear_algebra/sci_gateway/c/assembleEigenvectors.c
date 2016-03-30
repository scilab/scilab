/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Michaël Baudin
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

#include "elementary_functions.h"
#include "elem_common.h"

extern int C2F(dlaset)();

//
// assembleEigenvectorsSourceToTarget --
//   Assemble conjugated eigenvectors from the real part into real and imaginary parts.
//   Dispatch the real part of the eigenvectors into the real and imaginary parts,
//   depending on the imaginary part of the eigenvalues.
//   The current function reorder the values in the eigenvector array
//   and convert from Lapack, compact storage to the Scilab, natural storage.
// Arguments
//   iRows : number of rows
//   eigenvaluesImg, input : the imaginary part of the eigenvalues
//   EVRealSource, input : the real parts of the source eigenvectors
//   EVRealTarget, output : real part of the target eigenvectors
//   EVImgTarget, output : imaginary part of the target eigenvectors
// Notes
//   In some eigenvalue computing routines, such as dggev for example,
//   the eigenvectors are :
//   - real (that is with an imaginary part = 0),
//   - conjugated.
//   In that case, Lapack stores the eigenvectors in the following compact way.
//   (Extracted from DGGEV comments)
//------------------------
//   The right eigenvectors v(j) are stored one
//   after another in the columns of VR, in the same order as
//   their eigenvalues. If the j-th eigenvalue is real, then
//   v(j) = VR(:,j), the j-th column of VR. If the j-th and
//   (j+1)-th eigenvalues form a complex conjugate pair, then
//   v(j) = VR(:,j)+i*VR(:,j+1) and v(j+1) = VR(:,j)-i*VR(:,j+1).
//------------------------
//   But in Scilab, the eigenvectors must be order in a more natural order,
//   and this is why a conversion must be performed.
//
int assembleEigenvectorsSourceToTarget(int iRows, double * eigenvaluesImg,
                                       double * EVRealSource,
                                       double * EVRealTarget, double * EVImgTarget)
{

    double ZERO = 0;
    int i;
    int ij;
    int ij1;
    int j;

    j = 0;
    while (j < iRows)
    {
        if (eigenvaluesImg[j] == ZERO)
        {
            for (i = 0 ; i < iRows ; i++)
            {
                ij = i + j * iRows;
                EVRealTarget[ij] = EVRealSource[ij];
                EVImgTarget[ij] = ZERO;
            }
            j = j + 1;
        }
        else
        {
            for (i = 0 ; i < iRows ; i++)
            {
                ij = i + j * iRows;
                ij1 = i + (j + 1) * iRows;
                EVRealTarget[ij] = EVRealSource[ij];
                EVImgTarget[ij] = EVRealSource[ij1];
                EVRealTarget[ij1] = EVRealSource[ij];
                EVImgTarget[ij1] = -EVRealSource[ij1];
            }
            j = j + 2;
        }
    }
    return 0;
}
//
// assembleEigenvectorsInPlace --
//   Assemble conjugated eigenvectors from the real part into real and imaginary parts.
//   Dispatch the real part of the eigenvectors into the real and imaginary parts,
//   depending on the imaginary part of the eigenvalues.
//   The current function reorder the values in the eigenvector array
//   and convert from Lapack, compact storage to the Scilab, natural storage.
//   Perform the assembling in place, that is, update the matrix.
// Arguments
//   iRows : number of rows
//   iCols : number of columns
//   eigenvaluesImg, input : the imaginary part of the eigenvalues
//   EVReal, input/output : real part of the eigenvectors
//   EVImg, output : imaginary part of the eigenvectors
//
int assembleEigenvectorsInPlace(int iRows, double * eigenvaluesImg,
                                double * EVReal, double * EVImg)
{

    double ZERO = 0;
    int j;
    int INCY;
    int totalsize;

    totalsize = iRows * iRows;

    INCY = 1;
    C2F(dset)(&totalsize, &ZERO, EVImg, &INCY);

    j = 0;
    INCY = 1;
    while (j < iRows)
    {
        if (eigenvaluesImg[j] == ZERO)
        {
            j = j + 1;
        }
        else
        {
            int i;
            int ij;
            int ij1;
            for (i = 0 ; i < iRows ; i++)
            {
                ij = i + j * iRows;
                ij1 = i + (j + 1) * iRows;
                EVImg[ij]   =   EVReal[ij1];
                EVImg[ij1]  = - EVReal[ij1];
                EVReal[ij1] =   EVReal[ij];
            }
            j = j + 2;
        }
    }
    return 0;
}
//
// assembleComplexEigenvaluesFromDoublePointer --
//   Assemble the source complex eigenvalues array into the target complex eigenvalues diagonal matrix.
// Arguments
//   iRows : number of rows
//   EVRealSource, input : the real parts of source eigenvalues array, with size iRows
//   EVImgSource, input : the imaginary parts of source eigenvalues array, with size iRows
//   EVRealTarget, output : the real parts of target eigenvalues matrix, with size iRows x iRows
//   EVImgTarget, output : the imaginary parts of target eigenvalues matrix, with size iRows x iRows
//
int assembleComplexEigenvaluesFromDoublePointer(int iRows, double * EVRealSource, double * EVImgSource,
        double * EVRealTarget, double * EVImgTarget)
{
    int INCY = 1;
    int i;
    int ii;
    int totalsize;
    double ZERO = 0;
    totalsize = iRows * iRows;

    C2F(dset)(&totalsize, &ZERO, EVRealTarget, &INCY);
    C2F(dset)(&totalsize, &ZERO, EVImgTarget, &INCY);

    for (i = 0 ; i < iRows ; i++)
    {
        ii = i + i * iRows;
        EVRealTarget[ii] = EVRealSource[i];
        EVImgTarget[ii] = EVImgSource[i];
    }
    return 0;
}

//
// assembleComplexEigenvaluesFromDoubleComplexPointer --
//   Assemble the source complex eigenvalues array into the target complex eigenvalues diagonal matrix.
// Arguments
//   iRows : number of rows
//   EVSource, input : the real and imaginary parts of source eigenvalues array, with size iRows
//   EVRealTarget, output : the real parts of target eigenvalues matrix, with size iRows x iRows
//   EVImgTarget, output : the imaginary parts of target eigenvalues matrix, with size iRows x iRows
//
int assembleComplexEigenvaluesFromDoubleComplexPointer(int iRows, doublecomplex * EVSource,
        double * EVRealTarget, double * EVImgTarget)
{
    // Transfert eigenvalues
    char UPLO = 'F';
    double ZERO = 0;
    int icol;
    int ii;
    C2F(dlaset)( &UPLO, &iRows, &iRows, &ZERO, &ZERO, EVRealTarget, &iRows );
    C2F(dlaset)( &UPLO, &iRows, &iRows, &ZERO, &ZERO, EVImgTarget, &iRows );
    //     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
    for (icol = 0 ; icol < iRows ; icol++)
    {
        ii = icol + icol * iRows;
        EVRealTarget[ii] = EVSource[icol].r;
        EVImgTarget[ii] = EVSource[icol].i;
    }
    return 0;
}
//
// assembleEigenvaluesFromDoublePointer --
//   Assemble the source real eigenvalues array into the target real eigenvalues diagonal matrix.
// Arguments
//   iRows : number of rows
//   EVSource, input : the source eigenvalues array, with size iRows
//   EVTarget, output : the target eigenvalues matrix, with size iRows x iRows
//
int assembleEigenvaluesFromDoublePointer(int iRows, double * EVSource, double * EVTarget)
{
    int icol = 0;
    int ii;
    char UPLO = 'F';
    double ZERO = 0;
    C2F(dlaset)( &UPLO, &iRows, &iRows, &ZERO, &ZERO, EVTarget, &iRows );
    //     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

    for (icol = 0 ; icol < iRows ; icol++)
    {
        ii = icol + icol * iRows;
        EVTarget[ii] = EVSource[icol];
    }
    return 0;
}
