/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
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

#include "sci_malloc.h"
#include "core_math.h" // Max
#include "machine.h" // C2F
#include "doublecomplex.h"
#include <stdio.h> // debug
#include <math.h> //sqrt
#include "numericconstants_interface.h"
#include "invert_matrix.h"

extern double C2F(dlange)(char const * norm, int const * piRows, int const * piCols
                          , double const *pData, int const * piLDData, double* pdblWork);
extern double C2F(zlange)(char const * norm, int const * piRows, int const * piCols
                          , double const *pData, int const * piLDData, double* pdblWork);
extern void C2F(zgetrf)( int const* piRows, int const* piCols, double * pData
                         , int const * piLDData, int* piPivot, int* piInfo);
extern void C2F(dgetrf)( int const* piRows, int const* piCols, double * pData
                         , int const * piLDData, int* piPivot, int* piInfo);
extern void C2F(zgecon)(char const * norm, int const* piCols, double const * pData
                        , int const * piLDDA, double const* pdblAnorm
                        , double* pdblRcond, double* pdblWork, double* pdblRWork, int* piInfo);
extern void C2F(dgecon)(char const * norm, int const* piCols, double const * pData
                        , int const * piLDDA, double const* pdblAnorm
                        , double* pdblRcond, double* pdblWork, int* piRWork, int* piInfo);

extern void C2F(zgetri)( int const* n, doublecomplex* a, int const* ldA, int const* iPiv, doublecomplex* work, int const* workSize, int* info);
extern void C2F(dgetri)( int const* n, double* a, int const* ldA, int const* iPiv, double* work, int const* workSize, int* info);


int iInvertMatrix(int iRows, int iCols, double* pData, int complexArg
                  , double * pdblRcond, int* piPivot, void* pWork
                  , int iWorkSize, double* pdblWork);


/*
  TODO replace ugly constant 17 with properly #define d (enum ed ? ) constant
  for malloc error.
*/
int iInvertMatrixM(int iRows, int iCols, double* pData, int complexArg, double* pdblRcond)
{
    int ret = 0;
    int* piPivot = (int*)MALLOC(iCols * sizeof(int));
    if (piPivot)
    {
        int iWorkSize = Max(1, 4 * iCols);
        void* pWork = NULL;
        if (complexArg)
        {
            pWork = MALLOC(iWorkSize * sizeof(doublecomplex));
        }
        else
        {
            pWork = MALLOC(iCols * sizeof(int));
        }

        if (pWork)
        {
            double* pdblWork = (double*)MALLOC(iWorkSize * sizeof(double) * (complexArg  ? 2 : 1));

            if (pdblWork)
            {
                ret = iInvertMatrix(iRows, iCols, pData, complexArg, pdblRcond, piPivot, pWork, iWorkSize, pdblWork);
                FREE(pdblWork);
            }
            else
            {
                ret = 17;// TODO: this is not a stack allocation pb anymore because we (tried to) use the heap
            }
            FREE(pWork);
        }
        else
        {
            // pWork alloc did not succeed
            ret = 17;
        }
        FREE(piPivot);
    }
    else
    {
        // piPivot alloc did not succeed
        ret = 17;
    }
    return ret;
}

int iInvertMatrix(int iRows, int iCols, double* pData, int complexArg, double * pdblRcond, int* piPivot, void* pWork, int iWorkSize, double* pdblWork)
{
    int iInfo;
    int ret = 0; // >0 erreur <0 warning
    /* ANORM = dlange( '1', M, N, stk(lA), M, stk(lDWORK) )
    computes  one norm of a matrix (maximum column sum)
    last work area is not used for norm '1'
    see http://publib.boulder.ibm.com/infocenter/clresctr/vxrx/topic/com.ibm.cluster.essl44.guideref.doc/am501_llange.html
    */
    /* using "1" to pass '1' to Fortran ("by ref"->pointer to char)*/
    double dblAnorm  = 0;
    if (complexArg)
    {
        dblAnorm = C2F(zlange)("1", &iRows, &iCols, pData, &iRows, NULL /*see comment above */);
    }
    else
    {
        dblAnorm = C2F(dlange)("1", &iRows, &iCols, pData, &iRows, pdblWork/*see above*/);
    }

    if (complexArg)
    {
        C2F(zgetrf)(&iRows, &iCols, pData, &iCols, piPivot, &iInfo);
    }
    else
    {
        C2F(dgetrf)(&iRows, &iCols, pData, &iCols, piPivot, &iInfo);
    }

    if (iInfo != 0)
    {
        if (iInfo > 0)
        {
            ret = 19;
        }
    }
    else
    {
        *pdblRcond = 0.;
        if (complexArg)
        {
            C2F(zgecon)("1", &iCols, pData, &iCols, &dblAnorm, pdblRcond, pdblWork, (double*)pWork, &iInfo);
        }
        else
        {
            C2F(dgecon)("1", &iCols, pData, &iCols, &dblAnorm, pdblRcond, pdblWork, (int*)pWork, &iInfo);
        }

        if (*pdblRcond <= sqrt(nc_eps()))
        {
            ret = -1;
        }

        if (complexArg)
        {
            C2F(zgetri)( &iCols, (doublecomplex*)pData, &iCols, piPivot, (doublecomplex*)pdblWork, &iWorkSize, &iInfo);
        }
        else
        {
            C2F(dgetri)( &iCols, pData, &iCols, piPivot, pdblWork, &iWorkSize, &iInfo);
        }
        /* surprisingly enough, the original Fortran code does not check returned iInfo ...*/
    }
    return ret;
}

