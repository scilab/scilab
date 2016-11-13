/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
 * Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
#include <string.h> /* memset */
#include "core_math.h"
#include "machine.h"
#include "sci_malloc.h"

#include "eigen.h"

/*
 * Auxiliary functions to expand Lapack compact representation of eigen vectors into scilab matrix
 * see implementation at the end of the file for API documentation
 */
static int assembleEigenvectorsSourceToTarget(int iRows, double * eigenvaluesImg,
        double * EVRealSource, double * EVRealTarget, double * EVImgTarget);
static int assembleEigenvectorsInPlace(int iRows, double * eigenvaluesImg, double * EVReal, double * EVImg);


static int iEigen2Complex(doublecomplex* pData1, doublecomplex* pData2, int iCols, doublecomplex* pAlpha, doublecomplex* pBeta
                          , doublecomplex* pR, doublecomplex* pL, doublecomplex* pWork, int iWorkSize, double* pRWork);

static int iEigen2Real(double* pData1, double* pData2, int iCols, double* pAlphaReal, double* pAlphaImg, double* pBeta, double* pR, double* pL, double* pWork, int iWorkSize);

static double* allocateDggevWorkspace(int iCols, int* pWorksize);


/*
 * LAPACK Fortran functions used to do the real work for eigen values / vectors of one matrix.
 */

extern void C2F(zheev)(char const JOBZ[1]/*'N'|'V'*/, char const UPLO[1]/*'U'|'L'*/, int const* N, doublecomplex* A
                       , int const* LDA, double* W, doublecomplex* WORK, int const* LWORK, double* RWORK, int* INFO);

extern void C2F(zgeev)(char const JOBVL[1]/*'N'|'V'*/, char const JOBVR[1]/*'N'|'V'*/, int const* N, doublecomplex* A
                       , int const* LDA, doublecomplex* W, doublecomplex* VL, int const* LDVL, doublecomplex* VR, int const* LDVR
                       , doublecomplex* WORK, int const* LWORK, double* RWORK, int* INFO);

extern void C2F(dsyev)( char const JOBZ[1]/*'N'|'V'*/, char const UPLO[1]/*'U'|'L'*/, int const* N, double* A
                        , int const* LDA, double* W, double* WORK, int const* LWORK, int* INFO);

extern void C2F(dgeev)(char const JOBVL[1]/*'N'|'V'*/, char const JOBVR[1]/*'N'|'V'*/, int const* N, double* A
                       , int const* LDA, double* WR, double* WI, double* VL, int const* LDVL, double* VR, int const* LDVR
                       , double* WORK, int const* LWORK, int* INFO);


/*
 * LAPACK Fortran functions used to do the real work for eigen values / vectors of two matrix.
 */


extern void C2F(dggev)(char const jobVL[1]/*'N'|'V'*/, char const jobVR[1]/*'N'|'V'*/, int const* n, double* a, int const* ldA
                       , double* b, int const* ldB, double* alphaR, double* alphaI, double* beta, double* vl, int const* ldVl, double* vr, int const* ldVr
                       , double* WORK, int const* LWORK, int* INFO);
extern void C2F(zggev)(char const jobVL[1]/*'N'|'V'*/, char const jobVR[1]/*'N'|'V'*/, int const* n, doublecomplex* a, int const* ldA
                       , doublecomplex* b, int const* ldB, doublecomplex* alpha, doublecomplex* beta, doublecomplex* vl, int const* ldVl
                       , doublecomplex* vr, int const* ldVr, doublecomplex* WORK, int const* LWORK, double * RWORK, int* INFO);


/*
 * external function to perform division of complex vectors
 */
extern int C2F(wwrdiv)(double *ar, double *ai, int const *ia, double *br, double *bi, int const *ib
                       , double *rr, double *ri, int const *ir, int const *n, int *ierr);


/*
 * Wrappers querying optimal worsksize and computing minimal worksize for LAPACK functions
 * @param iCols int nb of rows/columns of the matrix
 * @param lhs int nb of expected outputs from the spec scilab function, only when useful to compute optimal worksize
 * out:
 *
 * @param optWorkSize int* points to the optimal worksize
 * @param minWorkSize int* points to the minimal worksize
 */
static int zheevWorkSizes(int iCols, int* optWorkSize, int* minWorkSize)
{
    int info = 0, query = -1;
    doublecomplex opt;
    C2F(zheev)("N", "U", &iCols, NULL, &iCols, NULL, &opt, &query, NULL, &info );
    *optWorkSize = (int) opt.r;
    *minWorkSize = Max(1, 2 * iCols - 1);
    return info;
}

static int zgeevWorkSizes(int iCols,  int lhs, int* optWorkSize, int* minWorkSize)
{
    int info = 0, query = -1;
    doublecomplex opt;
    C2F(zgeev)("N", (lhs == 1 ? "N" : "V"), &iCols, NULL, &iCols, NULL, NULL, &iCols, NULL, &iCols, &opt, &query, NULL, &info );
    *optWorkSize = (int)opt.r;
    *minWorkSize = Max(1, 2 * iCols);
    return info;
}

static int dsyevWorkSizes(int iCols, int* optWorkSize, int* minWorkSize)
{
    int info = 0, query = -1;
    double opt;
    C2F(dsyev)("N", "U", &iCols, NULL, &iCols, NULL, &opt, &query, &info);
    *optWorkSize = (int)opt;
    *minWorkSize = Max(1, 3 * iCols - 1);
    return info;
}

static int dgeevWorkSizes(int iCols, int lhs, int* optWorkSize, int* minWorkSize)
{
    int info = 0, query = -1;
    double opt;
    C2F(dgeev)("N", "N", &iCols, NULL, &iCols, NULL, NULL, NULL, &iCols, NULL, &iCols, &opt, &query, &info);
    *optWorkSize = (int)opt;

    *minWorkSize = (lhs == 2) ? Max(1, 4 * iCols) : Max(1, 3 * iCols);
    return info;
}

/**
 * try to MALLOC optimal (ws[0]) or at least minimal (ws[1]) number of doublecomplex ou double (according to isCplx arg)
 * @param int const ws[2] in : [0] is the optimal number of elements [1] the minimal
 * @param int (bool really) isCplx in : if true, allocate for doublecomplex elements, otherwise for double
 *
 * @param int* allocated out : nb of allocated elements
 * @return adress of MALLOCated memory or NULL
 */
static void* allocWorkspace(int const ws[2], int const isCplx, int* allocated)
{
    int i;
    void* res = NULL;
    for (i = 0; res == NULL && i != 2; ++i)
    {
        res = MALLOC(ws[i] * (isCplx ? sizeof(doublecomplex) : sizeof(double)));
    }
    *allocated = (res == NULL ? 0 : ws[i - 1]);
    return res;
}

/*
 * internal wrappers around LAPACK function calls
 * For symmetric cases, we use use an int (bool really) computeEigenVectors to express whether eigenvalues should be computed.
 * For unsymmetric cases, eigenvectors are not computed in place, so a NULL pointer for ouput indicates that eigen vectors should not be computed
 *
 * @param pData double[complex]* ptr to data matrix for symmetric matrix, also used as output for eigen vectors (when computed)
 * @param iCols int nb of rows/cols
 * @param computeEigenVectors int (boolean semantics) weither eigen vectors should be computed only used for symmetric data (cf. supra)

 * for symetric matrix, eigen values are real
 * @param pEigenValues double* out ptr to output eigen values

 * for unsymmetric matrix, eigen values are complex :
 *  in'c' format for unsymmetric real matrix :
 * @param pEigenValuesReal double*
 * @param pEigenValuesImg double*
 * in 'z' format for unsymmetric complex matrix
 * @param pEigenValues doublecomplex*

 * @param pRightVectors double[complex]* output eigen vectors (for unsymmetric matrix), when NULL, eigen vectors are not computed

 *
 * @param pWork doublecomplex* scratch ptr to workspace
 * @param iWorkSize int size of workspace
 * @param pRWork double* scratch workspace : only used for complex data
 */
static int iEigen1CS(doublecomplex* pData, int iCols, int computeEigenVectors, double* pEigenValues, doublecomplex* pWork, int iWorkSize, double* pRWork)
{
    int info;
    C2F(zheev)( computeEigenVectors ? "V" : "N", "U", &iCols, pData, &iCols, pEigenValues, pWork, &iWorkSize, pRWork, &info );
    return info;
}

static int iEigen1C(doublecomplex* pData, int iCols, doublecomplex* pEigenValues, doublecomplex* pRightVectors
                    , doublecomplex* pWork, int iWorkSize, double* pRWork)
{
    int info;
    C2F(zgeev)( "N", (pRightVectors == NULL ? "N" : "V"), &iCols, pData, &iCols, pEigenValues, NULL, &iCols, pRightVectors, &iCols
                , pWork, &iWorkSize, pRWork, &info );
    return info;
}

static int iEigen1RS(double* pData, int iCols, int computeEigenVectors, double* pEigenValues, double* pWork, int iWorkSize)
{
    int info;
    C2F(dsyev)( (computeEigenVectors ? "V" : "N"), "U", &iCols, pData, &iCols, pEigenValues, pWork, &iWorkSize, &info );
    return info;
}

static int iEigen1R(double* pData, int iCols, double* pEigenValuesReal, double* pEigenValuesImg, double* pRightVectors
                    , double* pWork, int iWorkSize)
{
    int info;
    C2F(dgeev)( "N", (pRightVectors == NULL ? "N" : "V"), &iCols, pData, &iCols, pEigenValuesReal, pEigenValuesImg
                , NULL, &iCols, pRightVectors, &iCols, pWork, &iWorkSize, &info );
    return info;
}


/********************************************************************************
 * functions used by client code to compute eigen values / vectors. See eigen.h *
 *******************************************************************************/

/*
 * Compute eigenvalues (and eigenvectors if computeEigenVectors is true) of a complex symmetric matrix.
 */
int iEigen1ComplexSymmetricM(doublecomplex* pData, int iCols, int computeEigenVectors, double* pEigenValues)
{
    int ret = 0;
    int ws[2];
    int worksize;
    zheevWorkSizes(iCols, ws, ws + 1);
    {
        doublecomplex* pWork;
        double* pRWork;
        pWork = (doublecomplex*)allocWorkspace(ws, 1, &worksize);
        pRWork = (double*)MALLOC(Max(1, 3 * iCols - 2) * sizeof(double));
        ret = (pWork && pRWork) ? iEigen1CS(pData, iCols, computeEigenVectors, pEigenValues, pWork, worksize, pRWork ) : 1;
        FREE(pRWork);
        FREE(pWork);
    }
    return ret;
}
/*
 * Compute eigenvalues (and eigenvectors if pEigenVectors is not NULL) of a complex unsymmetric matrix.
 */
int iEigen1ComplexM(doublecomplex* pData, int iCols, doublecomplex* pEigenValues, doublecomplex* pEigenVectors)
{
    int ret = 0;
    int ws[2];
    int worksize;
    int lhs = (pEigenVectors == NULL ? 1 : 2);
    zgeevWorkSizes(iCols, lhs, ws, ws + 1);
    {
        doublecomplex* pWork;
        double* pRWork;
        pWork = (doublecomplex*)allocWorkspace(ws, 1, &worksize);
        pRWork = (double*)MALLOC(2 * iCols * sizeof(double));
        ret = (pWork && pRWork ) ? iEigen1C(pData, iCols, pEigenValues, pEigenVectors, pWork, worksize, pRWork) : 1 ;
        FREE(pWork);
        FREE(pRWork);
    }
    return ret;
}
/*
 * Compute eigenvalues (and eigenvectors if computeEigenVectors is true) of a complexreal symmetric matrix.
 */
int iEigen1RealSymmetricM(double* pData, int iCols, int computeEigenVectors, double* pEigenValues)
{
    int ret = 0;
    int ws[2];
    int worksize;
    dsyevWorkSizes(iCols, ws, ws + 1);
    {
        double* pWork;
        pWork = allocWorkspace(ws, 0, &worksize);
        ret = (pWork) ? iEigen1RS(pData, iCols, computeEigenVectors, pEigenValues, pWork, worksize ) : 1 ;
        FREE(pWork);
    }
    return ret;
}
/*
 * Compute eigenvalues (and eigenvectors if pEigenVectorsReal is not NULL) of a real unsymmetric matrix.
 */
int iEigen1RealM(double* pData, int iCols, double* pEigenValuesReal, double* pEigenValuesImg, double* pEigenVectorsReal, double* pEigenVectorsImg)
{
    int ret = 0;
    int ws[2];
    int worksize;
    int lhs = (pEigenVectorsReal == NULL ? 1 : 2);
    dgeevWorkSizes(iCols, lhs, ws, ws + 1);
    {
        double* pWork;
        double* pRightVectors;
        pWork = allocWorkspace(ws, 0, &worksize);
        pRightVectors =  (lhs == 2 ? (double*)MALLOC(iCols * iCols * sizeof(double)) : NULL );
        iEigen1R(pData, iCols, pEigenValuesReal, pEigenValuesImg, pRightVectors, pWork, worksize);
        FREE(pWork);
        if (lhs == 2)
        {
            assembleEigenvectorsSourceToTarget(iCols, pEigenValuesImg,
                                               pRightVectors,
                                               pEigenVectorsReal, pEigenVectorsImg);
            FREE(pRightVectors);
        }
    }
    return ret;
}



/*
 * done in place because a matrix is aways larger than the vector of it's diagonal elements. We must copy from the end to avoid
 * overwriting elements.
 * Part of the API: cf eigen.h
 */
void expandToDiagonalOfMatrix(double* pData, int iCols)
{
    double* ptrDest = pData + iCols * iCols;
    double const* ptrSrc = pData + iCols;
    while (--ptrSrc != pData) /* last (first in fact, as we must go backward) diagonal element does not need to be copied as it is already in place*/
    {
        *(--ptrDest) = *ptrSrc;
        ptrDest -= iCols;
        memset(ptrDest, 0, iCols * sizeof(double));
    }
}

/*
 * complex case is not done inplace because real or imaginary 1x1 matrix are smaller than their complex diagonal.
 * Part of the API: cf eigen.h
 */
void expandZToDiagonalOfCMatrix(doublecomplex const* pZVector, int iCols, double* pRMatrix, double* pIMatrix)
{
    double* ptrDestR = pRMatrix + iCols * iCols;
    double* ptrDestI = pIMatrix + iCols * iCols;
    double const* ptrSrc = (double const *)(pZVector + iCols);
    /* we handle the last (in fact first) diagonal element out of the loop because then no memset is needed */
    double const* const end = (double const * const)(pZVector + 1);
    while (ptrSrc != end)
    {
        *(--ptrDestI) = *(--ptrSrc);
        *(--ptrDestR) = *(--ptrSrc);
        ptrDestI -= iCols;
        memset(ptrDestI, 0, iCols * sizeof(double));
        ptrDestR -= iCols;
        memset(ptrDestR, 0, iCols * sizeof(double));
    }
    /* copy first diagonal element */
    *pIMatrix = *(--ptrSrc);
    *pRMatrix = *(--ptrSrc);
}

/*
 * Wrappers allocation workspace (after querying optimal worsksize and computing minimal worksize) for LAPACK functions handling two matrix
 * (contrary to one matrix, there is no sense in sparating query and allocation as it would not improve code factorisation)
 *
 * @param iCols int nb of rows/columns of the matrix
 *
 * out:
 *
 * @param pWorkSize int* nb of allocated elements
 *
 *@return double[complex]* ptr to allocated workspace (NULL if malloc failed)
 */
static double* allocateDggevWorkspace(int iCols, int* pWorksize)
{
    double* ret = NULL;
    int info;
    int query = -1;
    double opt;

    C2F(dggev)("N", "N", &iCols, NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, NULL, &iCols, NULL, &iCols, &opt, &query, &info);

    *pWorksize = (int)opt;
    ret = MALLOC(*pWorksize * sizeof(double));

    if (!ret)
    {
        *pWorksize = Max(1, 8 * iCols);
        ret = MALLOC(*pWorksize * sizeof(double));
        if (!ret)
        {
            *pWorksize = 0;
        }
    }
    return ret;
}
static doublecomplex* allocateZggevWorkspace(int iCols, int* pWorksize)
{
    doublecomplex* ret = NULL;
    int info;
    int query = -1;
    doublecomplex opt;

    C2F(zggev)("N", "N", &iCols, NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, &iCols, NULL, &iCols, &opt, &query, NULL, &info);

    *pWorksize = (int) opt.r;
    ret = MALLOC(*pWorksize * sizeof(double));

    if (!ret)
    {
        *pWorksize = Max(1, 8 * iCols);
        ret = MALLOC(*pWorksize * sizeof(double));
        if (!ret)
        {
            *pWorksize = 0;
        }
    }
    return ret;
}

/*
 * internal wrappers around LAPACK function calls
 * For symmetric cases, we use use an int (bool really) computeEigenVectors to express whether eigenvalues should be computed.
 * For unsymmetric cases, eigenvectors are not computed in place, so a NULL pointer for ouput indicates that eigen vectors should not be computed
 *
 * @param pData double[complex]* ptr to data matrix for symmetric matrix, also used as output for eigen vectors (when computed)
 * @param iCols int nb of rows/cols
 * @param computeEigenVectors int (boolean semantics) weither eigen vectors should be computed only used for symmetric data (cf. supra)

 * for symetric matrix, eigen values are real
 * @param pEigenValues double* out ptr to output eigen values

 * for unsymmetric matrix, eigen values are complex :
 *  in'c' format for unsymmetric real matrix :
 * @param pEigenValuesReal double*
 * @param pEigenValuesImg double*
 * in 'z' format for unsymmetric complex matrix
 * @param pEigenValues doublecomplex*

 * @param pRightVectors double[complex]* output eigen vectors (for unsymmetric matrix), when NULL, eigen vectors are not computed

 *
 * @param pWork doublecomplex* scratch ptr to workspace
 * @param iWorkSize int size of workspace
 * @param pRWork double* scratch workspace : only used for complex data
 */

static int iEigen2Complex(doublecomplex* pData1, doublecomplex* pData2, int iCols, doublecomplex* pAlpha, doublecomplex* pBeta, doublecomplex* pR, doublecomplex* pL, doublecomplex* pWork, int iWorkSize, double* pRWork)
{
    int info;
    C2F(zggev)((pL != NULL) ? "V" : "N", (pR != NULL) ? "V" : "N", &iCols, pData1, &iCols, pData2, &iCols, pAlpha, pBeta, pL, &iCols, pR, &iCols, pWork, &iWorkSize, pRWork, &info);
    return info;
}

static int iEigen2Real(double* pData1, double* pData2, int iCols, double* pAlphaReal, double* pAlphaImg, double* pBeta, double* pR, double* pL, double* pWork, int iWorkSize)
{
    int info;
    C2F(dggev)((pL != NULL) ? "V" : "N", (pR != NULL) ? "V" : "N", &iCols, pData1, &iCols, pData2, &iCols, pAlphaReal, pAlphaImg, pBeta, pL, &iCols, pR, &iCols, pWork, &iWorkSize, &info);
    return info;
}

/******************************************************************************
 * Part of the API: cf eigen.h
 ******************************************************************************/
int iEigen2ComplexM(doublecomplex* pData1, doublecomplex* pData2, int iCols, doublecomplex* pAlpha, doublecomplex* pBeta, doublecomplex* pR, doublecomplex* pL)
{
    int ret = 0;
    int iRWorkSize = 0;
    int worksize;
    double* pRWork = NULL;
    doublecomplex* pWork = NULL;
    int onlyOneLhs = (pBeta == NULL); /* if beta was not requested (only one lhs), memory was not provided for beta, but will be needed to scale alpha */
    iRWorkSize = Max(1, 8 * iCols);

    ret =   ((pBeta = (onlyOneLhs ? (doublecomplex*)MALLOC(iCols * sizeof(doublecomplex)) : pBeta)) == NULL)
            || ((pRWork = (double*)MALLOC(iRWorkSize * sizeof(double))) == NULL)
            || ((pWork = allocateZggevWorkspace(iCols, &worksize)) == NULL);

    /* use explicit -1 to signal malloc error as >0 codes are used for convergence troubles */
    ret = ret ? -1 : iEigen2Complex(pData1, pData2, iCols, pAlpha, pBeta, pR, pL, pWork, worksize, pRWork);

    if ((ret >= 0) && (ret <= iCols) && onlyOneLhs)
    {
        /* no error, maybe warnings  and only one lhs : adjust alpha */
        int ierr; /* original code doe not check ierr */

        /* in fact it's (&pAlpha[1].r - &pAlpha[1].r) / sizeof(double), or... 2 :) */
        int const delta = sizeof(doublecomplex) / sizeof(double);

        C2F(wwrdiv)(&pAlpha[0].r, &pAlpha[0].i, &delta, &pBeta[0].r, &pBeta[0].i, &delta, &pAlpha[0].r, &pAlpha[0].i, &delta, &iCols, &ierr);
    }

    FREE(pRWork);
    FREE(pWork);

    if (onlyOneLhs)
    {
        FREE(pBeta);
    }
    return ret;
}

/******************************************************************************
 * Part of the API: cf eigen.h
 ******************************************************************************/
int iEigen2RealM(double* pData1, double* pData2, int iCols, double* pAlphaReal, double* pAlphaImg, double* pBeta, double* pRReal, double* pRImg, double* pLReal, double* pLImg)
{
    int ret = 0;
    int worksize;
    double* pWork = NULL;
    int onlyOneLhs = (pBeta == NULL);

    ret =   ((pBeta = (onlyOneLhs ? (double*)MALLOC(iCols * sizeof(double)) : pBeta)) == NULL)
            ||  ((pWork = allocateDggevWorkspace(iCols, &worksize)) == NULL);

    // use explicit -1 to signal malloc error as >0 codes are used for convergence troubles
    ret = ret ? -1 : iEigen2Real(pData1, pData2, iCols, pAlphaReal, pAlphaImg, pBeta, pRReal, pLReal, pWork, worksize);

    if ((ret >= 0) && (ret <= iCols))
    {
        // no error, maybe warnings  and only one lhs : adjust alpha
        if (onlyOneLhs)
        {
            int i;
            for (i = 0; i != iCols; ++i)
            {
                pAlphaReal[i] /= pBeta[i];
                pAlphaImg[i] /= pBeta[i];
            }
        }
        if (pRReal)
        {
            assembleEigenvectorsInPlace(iCols, pAlphaImg, pRReal, pRImg);
        }
        if (pLReal)
        {
            assembleEigenvectorsInPlace(iCols, pAlphaImg, pLReal, pLImg);
        }
    }

    FREE(pWork);
    if (onlyOneLhs)
    {
        FREE(pBeta);
    }
    return ret;
}



/******************************************************************************
 * Code below lifted from assembleEigenvectors.c
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Michaël Baudin
 *
 ******************************************************************************/
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
static int assembleEigenvectorsSourceToTarget(int iRows, double * eigenvaluesImg,
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
static int assembleEigenvectorsInPlace(int iRows, double * eigenvaluesImg, double * EVReal, double * EVImg)
{

    double ZERO = 0;
    int j;
    int INCY;
    int totalsize;

    totalsize = iRows * iRows;

    INCY = 1;
    /*	C2F(dset)(&totalsize,&ZERO,EVImg,&INCY);*/
    memset(EVImg, 0, totalsize * sizeof(double));
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
