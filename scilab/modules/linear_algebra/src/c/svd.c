/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "doublecomplex.h"
#include "machine.h"
#include "sci_malloc.h"
#include "core_math.h"
#include "svd.h"
#include "numericconstants_interface.h"

/*
 * Lapack functions performing the real work
 */
extern void C2F(zgesvd)(char const jobU[1]/* 'A'|'S'|'O'|'N'*/, char const jobVT[1]/* 'A'|'S'|'O'|'N'*/, int const* iRows, int const* iCols
                        , doublecomplex* a, int const* ldA, double* s, doublecomplex* u, int const* ldU, doublecomplex* vt, int const* ldVT
                        , doublecomplex* work, int const* lWork, double* rWork, int* info);
extern void C2F(dgesvd)(char const jobU[1]/* 'A'|'S'|'O'|'N'*/, char const jobVT[1]/* 'A'|'S'|'O'|'N'*/, int const* iRows, int const* iCols
                        , double* a, int const* ldA, double* s, double* u, int const* ldU, double* vt, int const* ldVT
                        , double* work, int const* lWork, int* info);

/*
 * Lapack utility functions
 */
/* sorting (used to correct a Lapack bug */
extern void C2F(dlasrt)(char const id[1]/* 'I'ncreasing|'D'ecreasing order */, int const* length, double* array, int* info);

/*
 * functions used to allocate workspace for Lapack functions
 * in: job char
 *
 */
static double* allocDgesvdWorkspace(char job, int iRows, int iCols, int iColsToCompute, int* iAllocated);
static int iDgesvd(char job, double* pData, int iRows, int iCols, int iColsToCompute, double* pSV, double* pU, double* pVT, double* pWork, int iWorkSize);
static doublecomplex* allocZgesvdWorkspace(char job, int iRows, int iCols, int computeA, int* iAllocated);
static int iZgesvd(char job, doublecomplex* pData, int iRows, int iCols, int iColsToCompute, double* pSV, doublecomplex* pU, doublecomplex* pVT
                   , doublecomplex* pWork, int iWorkSize, double* pRWork);

/* original fortran code says this is needed to workaround a bug in lapack, should assess necessity for current Lapack*/
static int iCorrectSV(double* pSV, int iMinRowsCols);
/* if lhs >1, S and V are computed from SV and VT */
static int iComputeSandVReal( double const* pSV, double const* pVT, int iRows, int iCols, int economy, double* pS, double* pV);
static int iComputeSandVComplex( double const* pSV, doublecomplex const* pVT, int iRows, int iCols, int economy, double* pS, doublecomplex* pV);


static doublecomplex* allocZgesvdWorkspace(char job, int iRows, int iCols, int iColsToCompute, int* iAllocated)
{
    doublecomplex* res = NULL;
    doublecomplex optimal;
    int query = -1; /* query zgesvd for optimal worksize */
    int info;
    C2F(zgesvd)(&job, &job, &iRows, &iCols, NULL, &iRows, NULL, NULL, &iRows, NULL, &iColsToCompute, &optimal, &query, NULL, &info);
    *iAllocated = (int) optimal.r;
    res = MALLOC((*iAllocated) * sizeof(doublecomplex)); /* try to allocate optimal worksize */
    if (!res) /* allocation failed, try with minimal worksize */
    {
        *iAllocated = 2 * Min(iRows, iCols) + Max(iRows, iCols);
        res = MALLOC((*iAllocated) * sizeof(doublecomplex));
        if (!res) /* failed again, report that we allocated 0 bytes */
        {
            *iAllocated = 0;
        }
    }
    return res;
}
static int iZgesvd(char job, doublecomplex* pData, int iRows, int iCols, int iColsToCompute, double* pSV, doublecomplex* pU, doublecomplex* pVT
                   , doublecomplex* pWork, int iWorkSize, double* pRWork)
{
    int info;
    C2F(zgesvd)(&job, &job, &iRows, &iCols, pData, &iRows, pSV, (pU ? pU : pData) , &iRows, (pVT ? pVT : pData), &iColsToCompute
                , pWork, &iWorkSize, pRWork, &info);
    return ( job == 'N' ) ? iCorrectSV(pSV, Min(iRows, iCols)) : info ;
}

static double* allocDgesvdWorkspace(char job, int iRows, int iCols, int iColsToCompute, int* iAllocated)
{
    double* res = NULL;
    double optimal;
    int query = -1;
    int info;
    C2F(dgesvd)(&job, &job, &iRows, &iCols, NULL, &iRows, NULL, NULL, &iRows, NULL, &iColsToCompute, &optimal, &query, &info);
    *iAllocated = (int) optimal;
    res = MALLOC((*iAllocated) * sizeof(double));
    if (!res)
    {
        *iAllocated = Max(3 * Min(iRows, iCols) + Max(iRows, iCols), 5 * Min(iRows, iCols)); /* Max & Min might have been more eficient as functions */
        res = MALLOC((*iAllocated) * sizeof(double));
        if (!res)
        {
            *iAllocated = 0;
        }
    }
    return res;
}

static int iDgesvd(char job, double* pData, int iRows, int iCols, int iColsToCompute, double* pSV, double* pU, double* pVT
                   , double* pWork, int iWorkSize)
{
    int info;
    C2F(dgesvd)(&job, &job, &iRows, &iCols, pData, &iRows, pSV, (pU ? pU : pData) , &iRows, (pVT ? pVT : pData), &iColsToCompute
                , pWork, &iWorkSize, &info);
    return ( job == 'N' ) ? iCorrectSV(pSV, Min(iRows, iCols)) : info ;
}

/*
  lhs==1 ->   pSV && !pU && !pS && !pV
  lhs==3|4 ->  !pSV && pU && pS && pV
  lhs==4 <-> pRk != NULL
 */

int iSvdM(double* pData, int iRows, int iCols, int complexArg, int economy, double tol, double* pSV, double* pU, double* pS, double* pV, double* pRk)
{
    int ret = 0, allocOK;
    int worksize;

    double* pWork = NULL;
    double* pRWork = NULL;
    double* pVT = NULL;
    double* _pSV = NULL;

    char const job = pU ? (economy ? 'S' : 'A') : 'N';
    int colsToCompute = (job == 'S') ? Min(iRows, iCols) : iCols;
    _pSV = pSV ? pSV : (double*) MALLOC(Min(iRows, iCols) * sizeof(double));
    if ( (allocOK = ( (pWork = (complexArg
                                ? (double*)allocZgesvdWorkspace(job, iRows, iCols, colsToCompute, &worksize)
                                : allocDgesvdWorkspace(job, iRows, iCols, colsToCompute, &worksize)))
                      && (!complexArg
                          || (pRWork = (double*) MALLOC(5 * Min(iRows, iCols) * sizeof(double))))
                      && (pVT = (pU
                                 ? (double*)MALLOC(colsToCompute * iCols * (complexArg ? sizeof(doublecomplex) : sizeof(double)))
                                 : pData)))))
    {
        ret = complexArg
              ? iZgesvd(job,  (doublecomplex*)pData, iRows, iCols, colsToCompute, _pSV, (doublecomplex*) pU, (doublecomplex*)pVT
                        , (doublecomplex*)pWork, worksize, pRWork)
              : iDgesvd(job, pData, iRows, iCols, colsToCompute, _pSV, pU, pVT, pWork, worksize);
        if ( (ret == 0) && pU)
        {
            /* openmp sections */
            /* openmp section */
            {
                int unused = complexArg
                             ? iComputeSandVComplex(_pSV, (doublecomplex*)pVT, iRows, iCols, economy, pS, (doublecomplex*)pV)
                             : iComputeSandVReal(_pSV, pVT, iRows, iCols, economy, pS, pV);
            }
            /* openmp section */
            {
                if (pRk) /* compute rank */
                {
                    int i, rk;
                    tol = (tol == 0.) ? (Max(iRows, iCols) * nc_eps() * _pSV[0]) : tol; /* original Fortran code does the strict fp compare */
                    rk = -1;
                    for (i = 0; i != Min(iRows, iCols); ++i)
                    {
                        if (_pSV[i] > tol)
                        {
                            rk = i;
                        }
                    }
                    *pRk = (double)(rk + 1);
                }
            }
        }
    }
    FREE(pWork);
    FREE(pRWork);
    if (pU)
    {
        FREE(pVT);
    }
    if (!pSV)
    {
        FREE(_pSV);
    }
    return allocOK ? ret : -1;
}


static int iCorrectSV(double* pSV, int iMinRowsCols)
{
    int i, info;
    for (i = 0; i != iMinRowsCols; ++i)
    {
        pSV[i] = Abs(pSV[i]); /* should be fabs in c99 */
    }
    C2F(dlasrt)("D", &iMinRowsCols, pSV, &info);
    return info;
}

static int iComputeSandVComplex( double const* pSV, doublecomplex const* pVT, int iRows, int iCols, int economy, double* pS, doublecomplex* pV)
{
    int i, j;
    int minRowsCols = Min(iRows, iCols);
    int economyCols = economy ? minRowsCols : iCols;
    int economyRows = economy ? minRowsCols : iRows;
    memset(pS, 0, economyRows * economyCols * sizeof(double));
    for (i = 0; i != minRowsCols; ++i)
    {
        pS[i + i * economyRows] = pSV[i];
    }
    /* multicore: with at least 2 caches, each half ( in !economy mode) should be done in it's on thread to avoid cache trashing */
    for (j = 0; j != economyCols; ++j)
    {
        for (i = 0; i != iCols; ++i)
        {
            pV[i + j * iCols].r = pVT[j + i * economyCols].r;
            pV[i + j * iCols].i = -pVT[j + i * economyCols].i;

            // usless condition ???
            //	  if(!economy) // economyCols == iCols
            //	    {
            //	      pV[j + i*economyCols].r= pVT[i + j*iCols].r;
            //	      pV[j + i*economyCols].i= -pVT[i + j*iCols].i;
            //	    }
        }
    }
    return 0;
}

static int iComputeSandVReal( double const* pSV, double const* pVT, int iRows, int iCols, int economy, double* pS, double* pV)
{
    int i, j;
    int minRowsCols = Min(iRows, iCols);
    int economyCols = economy ? minRowsCols : iCols;
    int economyRows = economy ? minRowsCols : iRows;

    memset(pS, 0, economyRows * economyCols * sizeof(double));
    for (i = 0; i != minRowsCols; ++i)
    {
        pS[i + i * economyRows] = pSV[i];
    }
    for (j = 0; j != economyCols; ++j)
    {
        for (i = 0; i != iCols; ++i)
        {
            pV[i + j * iCols] = pVT[j + i * economyCols];
            // usless condition ???
            //	  if(!economy) // economyCols == iCols
            //	    {
            //	      pV[j + i*iCols]= pVT[i + j*iCols];
            //	    }
        }
    }
    return 0;
}
