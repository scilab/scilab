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

#include <string.h>
#include "machine.h"
#include "core_math.h"
#include "sci_malloc.h"
#include "doublecomplex.h"
#include "hess.h"

extern void C2F(dgehrd)(int const * piN, int const * piLo, int const * piHi, double* pA, int const * piLDA, double* pTau
                        , double* pWork, int const * piLWork, int * piInfo);
extern void C2F(zgehrd)(int const * piN, int const * piLo, int const * piHi, double* pA, int const * piLDA, double* pTau
                        , double* pWork, int const * piLWork, int * piInfo);

extern void C2F(dorghr)(int const * piN, int const * piLo, int const * piHi, double* pA, int const * piLDA, double* pTau
                        , double* pWork, int const * piLWork, int * piInfo);
extern void C2F(zunghr)(int const * piN, int const * piLo, int const * piHi, double* pA, int const * piLDA, double* pTau
                        , double* pWork, int const * piLWork, int * piInfo);

extern void C2F(zlacpy)(char const * uplo /* "U"pper, "L"ower, or full*/, int const * piRows, int const * piCols
                        , double const* pdblSource, int const * piLDSource, double* pdblDest, int const* piLDDest);

extern void C2F(dlacpy)(char const * uplo /* "U"pper, "L"ower, or full*/, int const * piRows, int const * piCols
                        , double const* pdblSource, int const * piLDSource, double* pdblDest, int const* piLDDest);

#define WORKING_Z_QUERIES 0

/* min = hi - lo * sizeof & Max(1,N) */
void workSizes(int n, int lo, int hi, int complexArg, int* bestSize, int* semiOptimalSize, int* minimalSize)
{
    double optHrd, optGhr;
    int query = -1;
    int info = 0;
    if ( WORKING_Z_QUERIES && complexArg)
    {
        C2F(zgehrd)(&n, &lo, &hi, NULL, &n, NULL, &optHrd, &query, &info);
        C2F(zunghr)(&n, &lo, &hi, NULL, &n, NULL, &optGhr, &query, &info);
    }
    else
    {
        C2F(dgehrd)(&n, &lo, &hi, NULL, &n, NULL, &optHrd, &query, &info);
        C2F(dorghr)(&n, &lo, &hi, NULL, &n, NULL, &optGhr, &query, &info);
    }
    *bestSize = (int)Max(optHrd, optGhr);
    *semiOptimalSize = (int)Min(optHrd, optGhr);
    *minimalSize = Max( (hi - lo), Max(1, n));
}


int iHessM(double * pData, int iCols, int complexArg, double* pH)
{
    /* dynamic allocation of Tau and Work */
    int ret = 0;
    int ws[3];
    int workSize;
    double* pWork = NULL;
    double* pTau =  NULL;
    workSizes(iCols, 1, iCols, complexArg, ws, ws + 1, ws + 2);
    {
        int i;
        for (i = 0; (pWork == NULL) && (i != 3); ++i)
        {
            workSize = ws[i];
            pWork = (double*) MALLOC( workSize * (complexArg ?  sizeof(doublecomplex) : sizeof(double))) ;
        }
    }
    if (pWork)
    {
        pTau = (double*) MALLOC( (iCols - 1) * (complexArg ?  sizeof(doublecomplex) : sizeof(double))) ;
    }
    if ( pWork && pTau )
    {
        ret = iHess(pData, iCols, complexArg, pH, pTau, pWork, workSize);
    }
    else
    {
        ret = 1; /* there should be an error cond enum for inner routines that should not depend on macro _ */
    }
    FREE(pWork);
    FREE(pTau);
    return ret;
}

int iHess(double* pData, int iCols, int complexArg, double* pH, double* pTau, double* pWork, int workSize)
{
    int const one = 1;
    int info;
    if (complexArg)
    {
        C2F(zgehrd)(&iCols, &one, &iCols, pData, &iCols, pTau, pWork, &workSize, &info);
    }
    else
    {
        C2F(dgehrd)(&iCols, &one, &iCols, pData, &iCols, pTau, pWork, &workSize, &info);
    }
    if (pH) /* -> Lhs == 2 */
    {
        /* TODO : replace useless [z|d]lapcy with memcpy(pH, pData, iCols * iCols * (complexArg ? sizeof(doublecomplex): sizeof(double))) ; */
        if (complexArg)
        {
            C2F(zlacpy)( "F", &iCols, &iCols, pData, &iCols, pH, &iCols);
        }
        else
        {
            C2F(dlacpy)( "F", &iCols, &iCols, pData, &iCols, pH, &iCols);
        }
    }
    if (iCols > 2)
    {
        int i, j;
        for (j = 0; j != iCols - 2; ++j)
        {
            for (i = j + 2; i != iCols; ++i)
            {
                if (complexArg)
                {
                    double* ptr = pData + 2 * (i + j * iCols);
                    *ptr = 0.;
                    *(ptr + 1) = 0.;
                }
                else
                {
                    *(pData + i + j * iCols) = 0.;
                }
            }
        }
    }
    if (pH) /* lhs == 2 */
    {
        if (complexArg)
        {
            C2F(zunghr)(&iCols, &one, &iCols, pH, &iCols, pTau, pWork, &workSize, &info);
        }
        else
        {
            C2F(dorghr)(&iCols, &one, &iCols, pH, &iCols, pTau, pWork, &workSize, &info);
        }
    }
    return info;
}
