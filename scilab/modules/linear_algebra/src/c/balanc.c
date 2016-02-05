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
#include "machine.h"
#include "doublecomplex.h"
#include "sci_malloc.h"
#include "balanc.h"

/* LAPACK fortran functions doing the real work  */
extern void C2F(dggbal)( char const Job[1]/*'N'|'P'|'S'|'B'*/, int const* n, double * a, int const * LDa, double* b, int const* LDb
                         , int* iLo, int* iHi, double* lScale, double* rScale, double* work, int* info);
extern void C2F(zggbal)( char const Job[1]/*'N'|'P'|'S'|'B'*/, int const* n, doublecomplex * a, int const * LDa, doublecomplex* b, int const* LDb
                         , int* iLo, int* iHi, double* lScale, double* rScale, double* work, int* info);
extern void C2F(dggbak)( char const Job[1]/*'N'|'P'|'S'|'B'*/, char const side[1]/*'R'|'L'*/, int const* iRows, int const* iLo, int const* iHi
                         , double const* lScale, double const* rScale, int const* iCols, double * v, int const* ldV, int* info);
extern void C2F(dgebak)( char const Job[1]/*'N'|'P'|'S'|'B'*/, char const side[1]/*'R'|'L'*/, int const* iRows, int const* iLo, int const* iHi
                         , double const* scale, int const* iCols, double * v, int const* ldV, int* info);
extern void C2F(dgebal)( char const Job[1]/*'N'|'P'|'S'|'B'*/, int const* n, double * a, int const * LDa, int* iLo, int* iHi
                         , double* Scale, int* info);
extern void C2F(zgebal)( char const Job[1]/*'N'|'P'|'S'|'B'*/, int const* n, doublecomplex * a, int const * LDa, int* iLo, int* iHi
                         , double* Scale, int* info);
/* LAPACK utility function used to create a diagonal unit matrix (should be recoded more efficiently in C with memset )*/
extern void C2F(dlaset)(char const uplo[1] /* "U"pper, "L"ower, or full*/, int const * piRows, int const * piCols
                        , double const * pAlpha, double const * pBeta, double* pData, int const * pLDData);

/* internal C wrapper around LAPACK functions, for 1 and 2 matrix */
static int iBalanc1(double* pData, int iCols, int complexArgs, double* lhsData, double* pScale);
static int iBalanc2(double* pData1, double* pData2, int iCols, int complexArgs, double* lhsData1, double* lhsData2
                    , double* pLScale, double* pRScale, double* pWork);

int iBalancM(double* pData1, double* pData2, int iCols, int complexArgs, double* lhsData1, double* lhsData2);

static int iBalanc1(double* pData, int iCols, int complexArgs, double* lhsData, double* pScale)
{
    int iLo, iHi;
    double one = 1., zero = 0.;
    int ret;
    ret = complexArgs
          ? (C2F(zgebal)("B", &iCols, (doublecomplex*)pData, &iCols, &iLo, &iHi, pScale, &ret), ret)
          : (C2F(dgebal)("B", &iCols, pData, &iCols, &iLo, &iHi, pScale, &ret), ret);
    C2F(dlaset)("F", &iCols, &iCols, &zero, &one, lhsData, &iCols);
    C2F(dgebak)("B", "R", &iCols, &iLo, &iHi, pScale, &iCols, lhsData, &iCols, &ret);
    return ret;
}
static int iBalanc2(double* pData1, double* pData2, int iCols, int complexArgs, double* lhsData1, double* lhsData2
                    , double* pLScale, double* pRScale, double* pWork)
{
    int iLo, iHi;
    double one = 1., zero = 0.;
    int ret;
    ret = complexArgs
          ? (C2F(zggbal)( "B", &iCols, (doublecomplex*)pData1, &iCols, (doublecomplex*)pData2, &iCols, &iLo, &iHi, pLScale, pRScale, pWork, &ret), ret)
          : (C2F(dggbal)( "B", &iCols, pData1, &iCols, pData2, &iCols, &iLo, &iHi, pLScale, pRScale, pWork, &ret), ret);
    C2F(dlaset)("F", &iCols, &iCols, &zero, &one, lhsData1, &iCols);
    C2F(dlaset)("F", &iCols, &iCols, &zero, &one, lhsData2, &iCols);
    C2F(dggbak)("B", "L", &iCols, &iLo, &iHi, pLScale, pRScale, &iCols, lhsData1, &iCols, &ret);
    C2F(dggbak)("B", "R", &iCols, &iLo, &iHi, pLScale, pRScale, &iCols, lhsData2, &iCols, &ret);
    return ret;
}

/* part of the API. cf balanc.h */
int iBalancM(double* pData1, double* pData2, int iCols, int complexArgs, double* lhsData1, double* lhsData2)
{
    int ret = 0;
    double* pScale = MALLOC(iCols * sizeof(double));
    if (pData2) /* Rhs == 2 && Lhs == 4 */
    {
        double* pRScale = NULL;
        double* pWork = NULL;
        if ( (pRScale =  MALLOC(iCols * sizeof(double))) && (pWork =  MALLOC(iCols * 6 * sizeof(double))) )
        {
            ret = iBalanc2(pData1, pData2, iCols, complexArgs, lhsData1, lhsData2, pScale, pRScale, pWork);
        }
        FREE(pRScale);
        FREE(pWork);
    }
    else
    {
        ret = iBalanc1(pData1, iCols, complexArgs, lhsData1, pScale);
    }
    FREE(pScale);
    return ret;
}
