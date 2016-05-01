
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Bernard HUGUENEY
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

#include <stdio.h>

#include "machine.h"
#include "doublecomplex.h"
#include "MALLOC.h"
#include "core_math.h"

#include "sas_schur.h"

/*
 * Lapack routines used to perform the schur decompositions
 */

extern void C2F(dgges)(char const jobVsL[1], char const jobVsR[1], char const sort[1],  dgges_select_t select, int const* n
                       , double* a, int const* ldA, double* b, int const* ldB, int* sDim, double* alphaR, double* alphaI, double* beta
                       , double* VSL, int const* ldVSL, double* VSR, int const* ldVSR, double* work, int const* lWork, int* bWork, int* info );

extern void C2F(dgees)(char const jobVs[1], char const sort[1], dgees_select_t select, int const* n, double* a, int const* ldA
                       , int * sDim, double* wR, double* wI, double* VS, int const* ldVS, double* work, int const* lWork, int* bWork, int* info );

extern void C2F(zgges)(char const jobVsL[1], char const jobVsR[1], char const sort[1],  zgges_select_t select
                       , int const* n, doublecomplex* a, int const* ldA, doublecomplex* b, int const* ldB, int* sDim
                       , doublecomplex* alpha, doublecomplex* beta, doublecomplex* VSL, int const* ldVSL, doublecomplex* VSR, int const* ldVSR
                       , doublecomplex* work, int const* lWork, double* rWork, int* bWork, int* info);

extern void C2F(zgees)(char const jobVs[1], char const sort[1],  zgees_select_t select, int const* n, doublecomplex* a, int const* ldA
                       , int * sDim, doublecomplex* w, doublecomplex* VS, int const* ldVS, doublecomplex* work, int const* lWork, double* rWork
                       , int* bWork, int* info );

extern double C2F(pythag)(double const* a, double const* b);

static double cplx_abs(doublecomplex const * ptrC)
{
    return C2F(pythag)((double const*)ptrC, ((double const*)(ptrC)) + 1);
}
/* /!\ maybe pythag would be safer but original Fortran code uses dlapy2 */
extern double C2F(dlapy2)(double const* a, double const*b);


/*
 * functions allocating workspace memory for schur decompositions : first try to allocate optimal workspace by querying Lapack. If MALLOC for optimal
 * worspace fail, try to allocate minimal workspace. If that also fails, return NULL.
 * Set *allocated to nb of allocated elements (double[complex]not bytes!).
 *
 * in :
 *
 * iCols int in : nb of rows/cols of the matri[x|ces]
 * computeSchurVectors int (boolean semantics) in : whether schur vectors are to be computed.
 *
 * out :
 *
 * allocated int* out: nb of allocated elements (double[complex]not bytes!).
 *
 * @return ptr to allocated workspace, NULL if allocation failed.
 */

static double* iAllocDggesWorkspace(int iCols, int computeSchurVectors, int* allocated)
{
    int info;
    int query = -1;
    double optim;
    double* ret = NULL;
    C2F(dgges)(computeSchurVectors ? "V" : "N", computeSchurVectors ? "N" : "V", "N", NULL, &iCols, NULL, &iCols, NULL, &iCols
               , NULL, NULL, NULL, NULL, NULL, &iCols, NULL, &iCols, &optim, &query, NULL, &info);
    *allocated = (int) optim;
    ret = (double*) MALLOC( (*allocated) * sizeof(double));
    if (!ret)
    {
        *allocated = 8 * iCols + 16;
        ret = (double*) MALLOC( (*allocated) * sizeof(double));
        if (!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}
static double* iAllocDgeesWorkspace(int iCols, int computeSchurVectors, int* allocated)
{
    int info;
    int query = -1;
    double optim;
    double* ret = NULL;
    C2F(dgees)(computeSchurVectors ? "V" : "N", "N", NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, NULL, &iCols, &optim, &query, NULL, &info);
    *allocated = (int) optim;
    ret = (double*) MALLOC( (*allocated) * sizeof(double));
    if (!ret)
    {
        *allocated = 3 * iCols;
        ret = (double*) MALLOC( (*allocated) * sizeof(double));
        if (!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}
static doublecomplex* iAllocZggesWorkspace(int iCols, int computeSchurVectors, int* allocated)
{
    int info;
    int query = -1;
    doublecomplex optim;
    doublecomplex* ret = NULL;
    C2F(zgges)(computeSchurVectors ? "V" : "N", computeSchurVectors ? "V" : "N", "N", NULL, &iCols, NULL, &iCols, NULL, &iCols
               , NULL, NULL, NULL, NULL, &iCols, NULL, &iCols, &optim, &query, NULL, NULL, &info);

    *allocated = (int) optim.r;
    ret = (doublecomplex*) MALLOC( (*allocated) * sizeof(doublecomplex));
    if (!ret)
    {
        *allocated = 2 * iCols;
        ret = (doublecomplex*) MALLOC( (*allocated) * sizeof(doublecomplex));
        if (!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}
static doublecomplex* iAllocZgeesWorkspace(int iCols, int computeSchurVectors, int* allocated)
{
    int info;
    int query = -1;
    doublecomplex optim;
    doublecomplex* ret = NULL;
    C2F(zgees)(computeSchurVectors ? "V" : "N", "N", NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, &iCols, &optim, &query, NULL, NULL, &info);
    *allocated = (int) optim.r;
    ret = (doublecomplex*) MALLOC( (*allocated) * sizeof(doublecomplex));
    if (!ret)
    {
        *allocated = 2 * iCols;
        ret = (doublecomplex*) MALLOC( (*allocated) * sizeof(doublecomplex));
        if (!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}

/*
 * part of the API. Cf. sas_schur.h
 */
int iSchurM(double* pData1, double* pData2, int iCols, int complexArgs, double* pLhsOpt1, double* pLhsOpt2, schur_select_type pSelectFun, int* pDim)
{
    int info = 0;
    int* pBwork = NULL;
    int worksize = 0;
    double* pRwork = NULL; // 'd' iCols
    doublecomplex* pCplxWork = NULL;

    int fallbackDim;

    pDim = pDim ? pDim : &fallbackDim;

    if ( (pBwork = (int*) MALLOC( ( pData2 ? 2 : 1) * iCols * sizeof(int))) )
    {
        char const * const jobL = pLhsOpt1 ? "V" : "N";
        char const * const jobR = pLhsOpt2 ? "V" : "N";
        char const * const sort = pSelectFun.none ? "S" : "N";
        //      fprintf(stderr," jobL=%s, jobR=%s, sort=%s, switch((complexArgs ? 1: 0) + (pData2 ? 2 : 0))=%d\n",jobL, jobR, sort, (complexArgs ? 1: 0) + (pData2 ? 2 : 0));
        switch ( (complexArgs ? 1 : 0) + (pData2 ? 2 : 0) )
        {

            case 1+2 : /* zgges */
            {
                doublecomplex* pAlpha = NULL;
                doublecomplex* pBeta = NULL;
                info = ( (pCplxWork = iAllocZggesWorkspace(iCols, pLhsOpt1 ? 1 : 0, &worksize))
                         && (pRwork = (double*) MALLOC(8 * iCols * sizeof(double)))
                         && (pAlpha = (doublecomplex*)MALLOC( iCols * sizeof(doublecomplex)))
                         && (pBeta = (doublecomplex*)MALLOC( iCols * sizeof(doublecomplex))))
                       ? ( C2F(zgges)(jobL, jobR, sort, pSelectFun.zgges_select, &iCols, (doublecomplex*)pData1, &iCols, (doublecomplex*)pData2, &iCols
                                      , pDim, pAlpha, pBeta, (doublecomplex*)pLhsOpt1, &iCols, (doublecomplex*)pLhsOpt2, &iCols
                                      , pCplxWork, &worksize, pRwork, pBwork, &info), info)
                       : -1;
                FREE(pAlpha);
                FREE(pBeta);
                break;
            }
            case 1+0: /* zgees */
            {
                doublecomplex* pW = NULL;
                info = ( (pCplxWork = iAllocZgeesWorkspace(iCols, pLhsOpt1 ? 1 : 0, &worksize))
                         && (pRwork = (double*) MALLOC( iCols * sizeof(double)))
                         && (pW = (doublecomplex*)MALLOC(iCols * sizeof(doublecomplex))))
                       ? (C2F(zgees)(jobL, sort, pSelectFun.zgees_select, &iCols, (doublecomplex*)pData1, &iCols, pDim, pW, (doublecomplex*)pLhsOpt1, &iCols
                                     , pCplxWork, &worksize, pRwork, pBwork, &info), info)
                       : -1;
                FREE(pW);
                break;
            }
            case 0+2: /* dgges */
            {
                double* pAlphaR = NULL;
                double* pAlphaI = NULL;
                double* pBeta = NULL;
                info = ( (pRwork = iAllocDggesWorkspace(iCols, pLhsOpt1 ? 1 : 0, &worksize))
                         && (pAlphaI = (double*) MALLOC(iCols * sizeof(double)))
                         && (pAlphaR = (double*) MALLOC(iCols * sizeof(double)))
                         && (pBeta = (double*) MALLOC(iCols * sizeof(double))) )
                       ? ( C2F(dgges)(jobL, jobR, sort, pSelectFun.dgges_select, &iCols, pData1, &iCols, pData2, &iCols, pDim, pAlphaR, pAlphaI, pBeta
                                      , pLhsOpt1, &iCols, pLhsOpt2, &iCols, pRwork, &worksize, pBwork, &info), info)
                       : -1;
                FREE(pAlphaR);
                FREE(pAlphaI);
                FREE(pBeta);
                break;
            }
            case 0+0: /* dgees */
            {
                double* pWR = NULL;
                double* pWI = NULL;
                info = ( (pRwork = iAllocDgeesWorkspace(iCols, pLhsOpt1 ? 1 : 0, &worksize))
                         && (pWR = (double*)MALLOC(iCols * sizeof(double)))
                         && (pWI = (double*)MALLOC(iCols * sizeof(double))))
                       ? (C2F(dgees)(jobL, sort, pSelectFun.dgees_select, &iCols, pData1, &iCols, pDim, pWR, pWI, pLhsOpt1, &iCols, pRwork, &worksize, pBwork, &info), info)
                       : -1;
                FREE(pWR);
                FREE(pWI);
                break;
            }
        }
        FREE(pBwork);
        FREE(pRwork);
        FREE(pCplxWork);
    }
    else
    {
        info = -1;
    }
    return info;
}

int zb02ow(doublecomplex const* alpha, doublecomplex const* beta)
{
    /* strict cmp to 0. in original fortran code, expr should be equivalent to DREAL(ALPHA/BETA).LT.ZERO */
    return (cplx_abs(beta) != 0.) ? (((alpha->r) * (beta->r) + (alpha->i) * (beta->i)) < 0.) : 0 ;
}

int zb02ox(doublecomplex const* alpha, doublecomplex const* beta)
{
    return cplx_abs(alpha) < cplx_abs(beta);
}

extern double C2F(dlamch)(char const query[1], long );

int sb02ow(double const* alphaReal, double const* alphaImg, double const* beta)
{
    /* original Fortran code does not use alphaImg aka  ALPHAI */
    return ( ((*alphaReal < 0.) && (*beta > 0.)) || ((*alphaReal > 0.) && (*beta < 0.))) && (Abs(*beta) > Abs(*alphaReal) * (C2F(dlamch)("p", 1L)));
}
int sb02ox(double const* alphaReal, double const* alphaImg, double const* beta)
{
    return C2F(dlapy2)(alphaReal, alphaImg) < Abs(*beta);
}
int zb02mv(doublecomplex const* w)
{
    return w->r < 0.;
}
int zb02mw(doublecomplex const* w)
{
    return cplx_abs(w) < 1.;
}
int sb02mv(double const* wReal, double const* wImg)
{
    /* original Fortran code does not use wImg aka IEIG */
    return *wReal < 0.;
}
int sb02mw(double const* wReal, double const* wImg)
{
    return C2F(dlapy2)(wReal, wImg) < 1.;
}

