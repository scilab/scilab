/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bernard HUGUENEY
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
#include "core_math.h"
#include "sci_malloc.h"
#include "lu.h"
#include "doublecomplex.h"

extern void C2F(zgetrf)(int*, int*, double*, int*, int*, int*);
extern void C2F(dgetrf)(int*, int*, double*, int*, int*, int*);
extern void C2F(dlaset)(char const uplo[1] /* "U"pper, "L"ower, or full*/, int const * piRows, int const * piCols
                        , double const * pAlpha, double const * pBeta, double* pData, int const * pLDData);
extern void C2F(dlaswp)(int const* n, double* a, int const* ldA, int const* k1, int const* k2, int const* iPiv, int const* incX);


int iLuM(double* pData, int iRows, int iCols, int complexArg, double* pdblLData, double* pdblUData, double* pdblEData )
{
    int ret;
    int* piPivot;
    int* piWork = NULL;
    int iMinRowsCols;
    double* pdblWork = NULL;
    iMinRowsCols = Min( iRows, iCols);
    piPivot = (int*) MALLOC(iMinRowsCols * sizeof(int));
    if (pdblEData == NULL) /* must allocate more tmp memory when not computing E */
    {
        piWork = (int*) MALLOC(iRows * sizeof(int));
        pdblWork = (double*) MALLOC(iRows * iMinRowsCols * ( complexArg ? sizeof(doublecomplex) : sizeof(double)));
    }
    ret = iLu(pData, iRows, iCols, complexArg, pdblLData, pdblUData, pdblEData, piPivot, piWork, pdblWork);
    if (pdblEData == NULL)
    {
        FREE( piWork);
        FREE( pdblWork );
    }
    return ret;
}
int iLu(double* pData, int iRows, int iCols, int complexArg, double* pdblLData, double* pdblUData, double* pdblEData
        , int* piPivot, int* piWork, double* pdblWork)
{
    int ret = 0 ;
    int iInfo;
    if (complexArg)
    {
        C2F(zgetrf)(&iRows, &iCols, pData, &iRows, piPivot, &iInfo);
    }
    else
    {
        C2F(dgetrf)(&iRows, &iCols, pData, &iRows, piPivot, &iInfo);
    }
    if (iInfo < 0)
    {
        ret = iInfo;
    }
    else
    {
        int iMinRowsCols;
        int i, j;
        double* pdCurrentL;
        double* pdCurrentU;
        double* pdCurrentArg;
        iMinRowsCols = Min(iRows, iCols);
        /* fill L matrix with 1.[+0i] on the diagonal */
        /*
        /!\ if E was not requested, we will have to swap rows before returning the result, so we do not
        fill the real L matrix, but pdblWork instead. We will then copy and swap rows from pdblWork to the result matrix.
              */
        pdCurrentL = (pdblWork == NULL) ? pdblLData : pdblWork ;
        pdCurrentArg = pData;
        for ( j = 0; j != iMinRowsCols; ++j)
        {
            for ( i = 0; i != iRows; ++i, ++pdCurrentL, ++pdCurrentArg)
            {
                if (i > j)
                {
                    *pdCurrentL = *pdCurrentArg;
                    if (complexArg)
                    {
                        /* copy imaginary part */
                        *++pdCurrentL = *++pdCurrentArg;
                    }
                }
                else
                {
                    *pdCurrentL = (i == j) ? 1. : 0.;
                    if (complexArg)
                    {
                        /* set imaginary part to 0. and advance pdCurrentArg */
                        *++pdCurrentL = 0.;
                        ++pdCurrentArg;
                    }
                }
            }
        }


        /* init U */
        pdCurrentU = pdblUData;
        pdCurrentArg = pData;
        for ( j = 0; j != iCols; ++j)
        {
            pdCurrentArg = pData + j * iRows * (complexArg ? 2 : 1);
            for ( i = 0; i != iMinRowsCols; ++i, ++pdCurrentU, ++pdCurrentArg)
            {
                if ( i <= j)
                {
                    *pdCurrentU = *pdCurrentArg;
                    if (complexArg)
                    {
                        /* copy imaginary part */
                        *++pdCurrentU = *++pdCurrentArg;
                    }
                }
                else
                {
                    *pdCurrentU = 0.;
                    if (complexArg)
                    {
                        *++pdCurrentU = 0.;
                        ++pdCurrentArg;
                    }
                }
            }
        }
        if ( pdblEData == NULL)
        {
            for (i = 0; i != iRows; ++i)
            {
                piWork[i] = i;
            }
            for (i = 0; i != iMinRowsCols; ++i)
            {
                int ip;
                ip = piPivot[i] - 1; /* -1 because piPivot contains Fortran index*/
                if ( ip != i)
                {
                    int swapTmp;
                    swapTmp = piWork[i];
                    piWork[i] = piWork[ip];
                    piWork[ip] = swapTmp;
                }
            }
            for (i = 0; i != iRows; ++i)
            {
                for (j = 0; j != iMinRowsCols; ++j)
                {
                    if (complexArg)
                    {
                        *(((doublecomplex*)pdblLData) + piWork[i] + j * iRows) = *(((doublecomplex*)pdblWork) + i + j * iRows);
                    }
                    else
                    {
                        *(pdblLData + piWork[i] + j * iRows) = *(pdblWork + i + j * iRows);
                    }
                }
            }
        }/* end if E was not requested */
        else
        {
            double const dblZero = 0., dblOne = 1.;
            int const iOne = 1;
            C2F(dlaset)("F", &iRows, &iRows, &dblZero, &dblOne, pdblEData, &iRows);
            C2F(dlaswp)(&iRows, pdblEData, &iRows, &iOne, &iMinRowsCols, piPivot, &iOne);
        }
    }/* end if iInfo reported an error in [z|d]getrf */
    return ret;
}
