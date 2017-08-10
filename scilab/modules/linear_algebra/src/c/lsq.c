
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2009 - INRIA
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
#include <string.h> /* memcpy */
#include "doublecomplex.h"
#include "machine.h"
#include "core_math.h"
#include "sci_malloc.h"
#include "numericconstants_interface.h"
#include "lsq.h"


extern void C2F(dgelsy)( int const* rows, int const* cols, int const* nRhs, double* a, int const* ldA, double* b, int const* ldB, int* jPvt
                         , double const* rCond, int* rank, double* work, int const* worksize, int* info);

extern void C2F(zgelsy)( int const* rows, int const* cols, int const* nRhs, doublecomplex* a, int const* ldA, doublecomplex* b, int const* ldB, int* jPvt
                         , double const* rCond, int* rank, doublecomplex* work, int const* worksize, double* rWork, int* info);

/*
   we [c|sh]ould use memcpy(void *dest, const void *src, size_t n), but matrix dimensions are not the same so we'd still have one loop.
   But this would be the case only for (at most) of the [d|z]lacpy calls because max(rows,cols)== at least one of {rows,cols}.
 */

extern void C2F(dlacpy)(char const * uplo /* "U"pper, "L"ower, or full*/, int const * piRows, int const * piCols
                        , double const* pdblSource, int const * piLDSource, double* pdblDest, int const* piLDDest);

extern void C2F(zlacpy)(char const * uplo /* "U"pper, "L"ower, or full*/, int const * piRows, int const * piCols
                        , doublecomplex const* pdblSource, int const * piLDSource, doublecomplex* pdblDest, int const* piLDDest);

/*
 * [try to] allocate workspace for [d|z]gelsy Lapack routines. First try to allocate optimal worksize (after querying Lapack for this size). If that fails
 * try to allocate minimal worksize (as per Lapack routines specifications). If that also fails, return NULL and set *allocated to 0.
 *
 * in :
 *
 * @param rows int : nb of rows of the matrix
 * @param cols int : nb of columnss of the matrix
 * @param nRhs int : nb of rhs of the system
 *
 * out :
 *
 * @param allocated int* : nb of _elements_ (double for allocDgelsyWorkspace, doublecomplex for allocZgelsyWorkspace) successfully allocated
 *
 * @return adr of the allocated workspace
 *
 */

static double* allocDgelsyWorkspace(int rows, int cols, int nRhs, int* allocated)
{
    double* ret = NULL;
    int info;
    double optim;
    int query = -1;
    int const maxRowsCols = Max(rows, cols);
    double dblfake = 0;
    int ifake = 0;

    C2F(dgelsy)(&rows, &cols, &nRhs, &dblfake, &rows, &dblfake, &maxRowsCols, &ifake, &dblfake, &ifake, &optim, &query, &info);
    *allocated = (int)optim;
    if ( !(ret = (double*)MALLOC(*allocated * sizeof(double))))
    {
        *allocated = Max( Min(rows, cols) + 3 * cols + 1, 2 * Min(rows, cols) + nRhs);
        if ( !(ret = (double*)MALLOC(*allocated * sizeof(double))))
        {
            *allocated = 0;
        }
    }
    return ret;
}

static doublecomplex* allocZgelsyWorkspace(int rows, int cols, int nRhs, int* allocated)
{
    doublecomplex* ret = NULL;
    int info;
    doublecomplex optim;
    int query = -1;
    int const maxRowsCols = Max(rows, cols);
    double dblfake = 0;
    int ifake = 0;
    doublecomplex cfake;

    C2F(zgelsy)(&rows, &cols, &nRhs, &cfake, &rows, &cfake, &maxRowsCols, &ifake, &dblfake, &ifake, &optim, &query, &dblfake, &info);
    *allocated = (int)optim.r;
    if ( !(ret = (doublecomplex*)MALLOC(*allocated * sizeof(doublecomplex))))
    {
        int const minRowsCols = Min(rows, cols);
        *allocated =  minRowsCols +  Max( Max( 2 * minRowsCols, cols + 1), minRowsCols + nRhs);
        if ( !(ret = (doublecomplex*)MALLOC(*allocated * sizeof(doublecomplex))))
        {
            *allocated = 0;
        }
    }
    return ret;
}
/* part of API. cf lsq.h */
int iLsqM(double* pData1, int iRows, int iCols, double* pData2, int iNRhs, int complexArgs, double* pResult, double* pTreshold, int* piRank)
{
    int ret = 0;
    double* pRwork = NULL;
    doublecomplex* pWork = NULL;
    double* pXb = NULL;
    int* pPivot = NULL;
    int worksize = 0 ;
    int unusedRank;
    double const treshold = pTreshold ? *pTreshold : sqrt(nc_eps());

    if ( (pRwork = (double*)( complexArgs ? (double*)MALLOC(2 * iCols * sizeof(double)) : allocDgelsyWorkspace(iRows, iCols, iNRhs, &worksize)))
            && (pXb = (double*)MALLOC(Max(iRows, iCols) * iNRhs * (complexArgs ? sizeof(doublecomplex) : sizeof(double))))
            && (pPivot = (int*)MALLOC(iCols * sizeof(int)))
            && (!complexArgs || (pWork = allocZgelsyWorkspace(iRows, iCols, iNRhs, &worksize))) )
    {
        int const maxRowsCols = Max(iRows, iCols);
        memset(pPivot, 0, iCols * sizeof(int));
        if (complexArgs)
        {
            /* cf supra : if(maxRowsCols == iRows){ memcpy(pXb, pData2, iRows * iNRhs) } else { zlacpy } */
            C2F(zlacpy)("F", &iRows, &iNRhs, (doublecomplex*)pData2, &iRows, (doublecomplex*)pXb, &maxRowsCols );
            C2F(zgelsy)(&iRows, &iCols, &iNRhs, (doublecomplex*)pData1, &iRows, (doublecomplex*)pXb, &maxRowsCols, pPivot
                        , &treshold, piRank ? piRank : &unusedRank, pWork, &worksize, pRwork, &ret);
            if (!ret)
            {
                /* cf supra : if(maxRowsCols == iCols){ memcpy(pResult, pXb, iCols * iNRhs) } else { zlacpy } */
                C2F(zlacpy)("F", &iCols, &iNRhs, (doublecomplex*)pXb, &maxRowsCols, (doublecomplex*)pResult, &iCols);
            }
        }
        else
        {
            C2F(dlacpy)("F", &iRows, &iNRhs, pData2, &iRows, pXb, &maxRowsCols );
            C2F(dgelsy)(&iRows, &iCols, &iNRhs, pData1, &iRows, pXb, &maxRowsCols, pPivot
                        , &treshold, piRank ? piRank : &unusedRank, pRwork, &worksize, &ret);
            if (!ret)
            {
                C2F(dlacpy)("F", &iCols, &iNRhs, pXb, &maxRowsCols, pResult, &iCols);
            }
        }
    }
    else
    {
        ret = -1; /* report MALLOC failure */
    }
    FREE(pRwork);
    FREE(pXb);
    FREE(pPivot);
    FREE(pWork);
    return ret;
}
