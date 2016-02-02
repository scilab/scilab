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
#include <string.h> /* for memset */
#include <stdio.h>

#include "core_math.h"
#include "sci_malloc.h"
#include "doublecomplex.h"
#include "qr.h"
#include "elem_common.h"

#define WORKING_ZQUERIES 0

extern void C2F(dgeqrf)(int const * piRows, int const * piCols, double* pData, int const * pLDData
                        , double* pdblTau, double* pdblWork, int const * piWorksize, int * piInfo);

extern void C2F(zgeqrf)(int const * piRows, int const * piCols, double* pData, int const * pLDData
                        , double* pdblTau, double* pdblWork, int const * piWorksize, int * piInfo);


extern void C2F(dgeqp3)(int const * piRows, int const * piCols, double* pData, int const * pLDData
                        , int* piPivot, double* pdblTau, double* pdblWork, int const * piWorksize, int * piInfo);

extern void C2F(zgeqp3)(int const * piRows, int const * piCols, double* pData, int const * pLDData
                        , int* piPivot, double* pdblTau, double* pdblWork, int const * piWorksize, double* pdblRWork, int * piInfo);

extern void C2F(dorgqr)(int const * piRows, int const * piCols, int* piK, double* pData, int const * pLDData
                        , double* pdblTau, double* pdblWork, int const * piWorksize, int * piInfo);

extern void C2F(zungqr)(int const * piRows, int const * piCols, int* piK, double* pData, int const * pLDData
                        , double* pdblTau, double* pdblWork, int const * piWorksize, int * piInfo);

extern void C2F(dlaset)(char const * uplo /* "U"pper, "L"ower, or full*/, int const * piRows, int const * piCols
                        , double const * pAlpha, double const * pBeta, double* pData, int const * pLDData);

extern double C2F(pythag)(double *a, double *b);

/* /!\ Performance considerations : deprecated lapack subroutines have been replaced by their
   more recent conterparts (introduced in version 3.0 released on June 30, 1999 )
   DGEQPF -> DGEQP3, ZGEQPF -> ZGEQP3
*/

/* /!\ workSize must be computed in a different way for SAS because :
   1) we cannot allocate all available memory
   2) performance is dependant on the amount of allocated memory

   So instead of claiming all available stack memory, we must perform a call on [d|z]geqrf with LWORK (=worksize)
   to query for optimal worksize.
   TODO: We could try a more sutble allocation stategy than falling back direclty from max(optimalWorkSizes) to max( minimalWorksizes)
*/
static int optimalWorkSizes(int complexArg, int iRows, int iCols, int iRowsToCompute
                            , int* opt1, int* opt2, int* opt3);
static int optimalWorkSizes(int complexArg, int iRows, int iCols, int iRowsToCompute
                            , int* opt1, int* opt2, int* opt3)
{
    double optGeqrf, optGeqp3, optGqr;
    int info = 0;
    int query = -1;
    int minRowsCols = Min(iRows, iCols); /* even when iRowsToCompute == iRows, [d|z]orgqr uses the min */
    if ( WORKING_ZQUERIES && complexArg) /* /!\ query mode for z lapack functions seems bugged ! */
    {
        C2F(zgeqrf)(&iRows, &iCols, /*unused A*/NULL, &iRows, /*unused Tau*/NULL, /*Work used for query*/ &optGeqrf, &query, &info);
        fprintf(stderr, "info:%d\n", info);
        info = 0;
        C2F(zgeqp3)(&iRows, &iCols, NULL, &iRows, NULL, NULL, &optGeqp3, &query, NULL, &info);
        fprintf(stderr, "info:%d\n", info);
        info = 0;
        C2F(zungqr)(&iRows, &iRowsToCompute, &minRowsCols, NULL, &iRows, NULL, &optGqr, &query, &info);
        fprintf(stderr, "info:%d\n", info);
    }
    else
    {
        C2F(dgeqrf)(&iRows, &iCols, NULL, &iRows, NULL, &optGeqrf, &query, &info);
        C2F(dgeqp3)(&iRows, &iCols, NULL, &iRows, NULL, NULL, &optGeqp3, &query, &info);
        C2F(dorgqr)(&iRows, &iRowsToCompute, &minRowsCols, NULL, &iRows, NULL, &optGqr, &query, &info);
    }
    /* sort sizes in decreasing order */
    *opt1 = (int)Max(optGeqrf, optGeqp3);
    *opt2 = (int)Min(optGeqrf, optGeqp3);
    if (optGqr > *opt1)
    {
        *opt3 = *opt2;
        *opt2 = *opt1;
        *opt1 = (int)optGqr;
    }
    else
    {
        if (optGqr > *opt2)
        {
            *opt3 = *opt2;
            *opt2 = (int)optGqr;
        }
        else
        {
            *opt3 = (int)optGqr;
        }
    }
    return info;
}

static int minimalWorkSize(int complexArg, int iRows, int iCols, int iRowsToCompute)
{
    /*call ZGEQRF( M, N, zstk(lA), M, //:LWORK >= max(1,N).
      call ZGEQPF( M, N, zstk(lA), M, istk(lJPVT), zstk(lTAU), ->zqeqp3: LWORK >= N+1.
      call ZUNGQR( M, min(M,N), min(M,N), zstk(lQ), M, zstk(lTAU),->:LWORK >= max(1,N). ici max (1, iRowsToCompute)

      call DGEQRF( M, N, stk(lA), M, stk(lTAU), stk(lDWORK), LWORK >= max(1,N).
      DGEQPF( M, N, stk(lA), M, istk(lJPVT), stk(lTAU)->dgeqp3 ,LWORK >= 3*N+1.

      call DORGQR( M, min(M,N), min(M,N), stk(lQ), M, stk(lTAU),LWORK >= max(1,N). ici max iRowsTocompute
    */
    int minGeqrf, minGeqp3, minGqr;
    minGeqrf = Max(1, iCols); /* lifted from original Fortran code, but should be useless after the previous tests
			      lifted from the same fortran code that are now in sci_qr. */
    minGeqp3 = (complexArg ? 3 : 1) * iCols + 1;
    minGqr = Max(1, iRowsToCompute);

    return Max( Max(minGeqrf, minGeqp3), minGqr);
}

static int iQr(double* pData, int iRows, int iCols,  int iRowsToCompute, double dblTol
               , double* pdblQ, double* pdblR, double* pdblE, double* pdblRank
               , int* piPivot, double* pdblTau, double* pdblWork, int iWorkSize, double* pdblRWork);
int iQrM(double* pData, int iRows, int iCols, int /*bool*/ complexArg, int iRowsToCompute, double dblTol
         , double* pdblQ, double* pdblR, double* pdblE, double* pdblRank)
{
    int ret;
    int* piPivot = NULL;
    double*  pdblTau = NULL;
    double* pdblWork = NULL; /* double ou doublecomplex */
    double* pdblRWork = NULL; /* used only for comlpexArg */
    int workSize = 0;
    {
        /* tying to alloc the optimal size of temporary memory for algorithms for biggest to smallest, trying minimal amount as last ressort*/
        int optWS[4];
        optimalWorkSizes(complexArg, iRows, iCols, iRowsToCompute, optWS, optWS + 1, optWS + 2);
        optWS[3] = minimalWorkSize(complexArg, iRows, iCols, iRowsToCompute);
        {
            int i;
            for (i = 0; (pdblWork == NULL) && (i != 4); ++i)
            {
                workSize = optWS[i];
                pdblWork = (double*) MALLOC(workSize * (complexArg ? sizeof(doublecomplex) : sizeof(double)));
            }
        }
    }
    if (!(ret =   !( pdblWork /* check previous allocation before trying (if not short-circuited) the next ones */
                     && (piPivot = (int*) MALLOC(iCols * sizeof(int)))
                     && (pdblTau = (double*) MALLOC( Min(iCols, iRows) * (complexArg ? sizeof(doublecomplex) : sizeof(double))))
                     && (!complexArg /* last alloc only needed for complex case */
                         || (pdblRWork = (double*) MALLOC( 2 * iCols * sizeof(double))))
                   )
         ))
    {
        ret = iQr(pData, iRows, iCols, iRowsToCompute, dblTol, pdblQ, pdblR, pdblE, pdblRank
                  , piPivot, pdblTau, pdblWork, workSize, pdblRWork);
    }
    FREE(pdblWork);
    FREE(piPivot);
    FREE(pdblTau);
    FREE(pdblRWork);
    return ret;
}

int iQr(double* pData, int iRows, int iCols,  int iRowsToCompute, double dblTol
        , double* pdblQ, double* pdblR, double* pdblE, double* pdblRank
        , int* piPivot, double* pdblTau, double* pdblWork, int iWorkSize, double* pdblRWork)
{
    int lhs = pdblRank ? 4 : (pdblE ? 3 : 2) ;
    int complexArg = !(pdblRWork == NULL);
    int economyMode = (iRowsToCompute < iRows);
    int info;

    memset(pdblR, 0, iRowsToCompute * iCols * (complexArg ? sizeof(doublecomplex) : sizeof(double)));
    if (lhs == 2)
    {
        if (complexArg)
        {
            C2F(zgeqrf)(&iRows, &iCols, pData, &iRows, pdblTau, pdblWork, &iWorkSize, &info);
        }
        else
        {
            C2F(dgeqrf)(&iRows, &iCols, pData, &iRows, pdblTau, pdblWork, &iWorkSize, &info);
        }
    }
    else
    {
        memset(piPivot, 0, iCols * sizeof(int));
        if (complexArg)
        {
            C2F(zgeqp3)(&iRows, &iCols, pData, &iRows, piPivot, pdblTau, pdblWork, &iWorkSize, pdblRWork, &info);
        }
        else
        {
            C2F(dgeqp3)(&iRows, &iCols, pData, &iRows, piPivot, pdblTau,  pdblWork, &iWorkSize, &info);
        }
    }
    if (info == 0)
    {
        if (complexArg)
        {
            C2F(zlacpy)("U", &iRowsToCompute, &iCols, pData, &iRows, pdblR, &iRowsToCompute);
        }
        else
        {
            C2F(dlacpy)("U", &iRowsToCompute, &iCols, pData, &iRows, pdblR, &iRowsToCompute);
        }
        if ( !economyMode && (iRows > iCols) )
        {
            if (complexArg)
            {
                C2F(zlacpy)("F", &iRows, &iCols, pData, &iRows, pdblQ, &iRows);
            }
            else
            {
                C2F(dlacpy)("F", &iRows, &iCols, pData, &iRows, pdblQ, &iRows);
            }
            {
                int i, j;
                for ( j = iCols; j != iRows; ++j)
                {
                    for ( i = 0; i != iRows; ++i)
                    {
                        if (complexArg)
                        {
                            double* const cplx = pdblQ + 2 * (i + j * iRows) ;
                            cplx[0] = 0.;
                            cplx[1] = 0.;
                        }
                        else
                        {
                            double* const ptr = pdblQ + (i + j * iRows) ;
                            *ptr = 0.;

                        }
                    }
                }
            }
        }
        else
        {
            if (complexArg)
            {
                C2F(zlacpy)("F", &iRows, &iRowsToCompute, pData, &iRows, pdblQ, &iRows);
            }
            else
            {
                C2F(dlacpy)("F", &iRows, &iRowsToCompute, pData, &iRows, pdblQ, &iRows);
            }
        }
        {
            int minRowsCols = Min(iRows, iCols);
            if (complexArg)
            {
                C2F(zungqr)(&iRows, &iRowsToCompute, &minRowsCols, pdblQ, &iRows, pdblTau
                            , pdblWork, &iWorkSize, &info);
            }
            else
            {
                C2F(dorgqr)(&iRows, &iRowsToCompute, &minRowsCols, pdblQ, &iRows, pdblTau
                            , pdblWork, &iWorkSize, &info);
            }
        }
        if ( lhs > 2 )
        {
            memset(pdblE, 0, iCols * iCols * sizeof(double));
            {
                int i, j;
                for (j = 0; j != iCols; ++j)
                {
                    i = piPivot[j] - 1;
                    pdblE[ i + j * iCols ] = 1.;
                }
            }
        }
        if ( lhs > 3)
        {
            double tt = complexArg ? C2F(pythag)(pdblR, pdblR + 1) : Abs(*pdblR) ;
            if ( dblTol < 0) /* /!\ original Frotran code does strict fp comparison with -1.0 */
            {
                dblTol = Max(iRows, iCols) * nc_eps_machine() * tt;
            }
            {
                int j, k = 0;
                double* ptrDiagR = pdblR; /* points to elements in the diagonal of R, hence the increment of iRows+1: +1 col & +1 row */
                int diagIncrement = (iRows + 1) * (complexArg ? 2 : 1);
                for (j = 0; (j != Min(iRows, iCols)) ; ++j, ptrDiagR += diagIncrement, ++k)
                {
                    if ((complexArg ? C2F(pythag)(ptrDiagR, ptrDiagR + 1) : Abs(*ptrDiagR)) < dblTol)
                    {
                        break;
                    }
                }
                *pdblRank = (double)k;
            }
        }
    }
    return info;
}
