
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
#include "machine.h"
#include "doublecomplex.h"
#include "sci_malloc.h"
#include "rcond.h"

/* Lapack routines doing the real work */
extern double C2F(zlange)(char const norm[1], int const* m, int const* n, doublecomplex const* a, int const* ldA, double* work, int* info);
extern double C2F(dlange)(char const norm[1], int const* m, int const* n, double const* a, int const* ldA, double* work, int* info);
extern void C2F(zgecon)(char const norm[1], int const* n, doublecomplex const* a, int const* ldA, double const* aNorm, double* rCond,
                        doublecomplex* work, double* rWork, int* info);
extern void C2F(dgecon)(char const norm[1], int const* n, double const* a, int const* ldA, double const* aNorm, double* rCond,
                        double* work, int* iWork, int* info);
extern void C2F(zgetrf)(int const* m, int const* n, doublecomplex* a, int const* ldA, int* iPiv, int* info);
extern void C2F(dgetrf)(int const* m, int const* n, double* a, int const* ldA, int* iPiv, int* info);

/* part of the API, cf. rcond.h */
int iRcondM(double* pData, int iCols, int complexArg, double* pRcond)
{
    int info;
    int* piPiv = (int*) MALLOC( iCols * sizeof(int));
    void* pRIWork = complexArg ? MALLOC(2 * iCols * sizeof(double)) : MALLOC(iCols * sizeof(int));
    double* pWork = (double*)MALLOC(complexArg ? (2 * iCols * sizeof(doublecomplex)) : (4 * iCols * sizeof(double)));
    if ( piPiv && pRIWork && pWork)
    {
        double aNorm = 0;
        if (complexArg)
        {
            aNorm = C2F(zlange)("1", &iCols, &iCols, (doublecomplex*)pData, &iCols, NULL, &info);
        }
        else
        {
            aNorm = C2F(dlange)("1", &iCols, &iCols, pData, &iCols, NULL, &info);
        }

        *pRcond = 0.;

        if (complexArg)
        {
            C2F(zgetrf)(&iCols, &iCols, (doublecomplex*)pData, &iCols, piPiv, &info);
            if (!info)
            {
                C2F(zgecon)("1", &iCols, (doublecomplex*)pData, &iCols, &aNorm, pRcond, (doublecomplex*)pWork, (double*)pRIWork, &info);
            }
        }
        else
        {
            C2F(dgetrf)(&iCols, &iCols, pData, &iCols, piPiv, &info);
            if (!info)
            {
                C2F(dgecon)("1", &iCols, pData, &iCols, &aNorm, pRcond, pWork, (int*)pRIWork, &info);
            }
        }
    }
    else
    {
        info = -1; /* MALLOC error */
    }
    FREE(piPiv);
    FREE(pRIWork);
    FREE(pWork);
    return info;
}
