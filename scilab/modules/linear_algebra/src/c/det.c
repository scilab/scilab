/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
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
#include "core_math.h"

#include "det.h"

extern void C2F(zgetrf)(int const* m, int const* n, doublecomplex* a, int const* ldA, int* iPiv, int* info);
extern void C2F(dgetrf)(int const* m, int const* n, double* a, int const* ldA, int* iPiv, int* info);

extern double C2F(pythag)(double *a, double *b); /* complex Abs */

int iDetM(double* pData, int iCols, double* pMantissaReal, double* pMantissaImg, int* piExponent)
{
    int info = 0;
    int* piPiv;
    if ( iCols == 0)
    {
        *pMantissaReal = 0.;
        if (pMantissaImg)
        {
            *pMantissaImg = 0.;
        }
        if (piExponent)
        {
            *piExponent = 1;
        }
    }
    else
    {
        if ( (piPiv = (int*) MALLOC(iCols * sizeof(int) )) )
        {
            if (pMantissaImg)
            {
                C2F(zgetrf)(&iCols, &iCols, (doublecomplex*)pData, &iCols, piPiv, &info);
            }
            else
            {
                C2F(dgetrf)(&iCols, &iCols, pData, &iCols, piPiv, &info);
            }
            if (info >= 0)
            {
                int i;
                int const base = 10;
                *pMantissaReal = 1.;
                if (piExponent)
                {
                    *piExponent = 0;
                }
                if (pMantissaImg)
                {
                    *pMantissaImg = 0.;
                    for (i = 0; i != iCols; ++i)
                    {
                        double a, b;
                        doublecomplex tmp = ((doublecomplex*)(pData))[i * (iCols + 1)];
                        if (piPiv[i] == i + 1)
                        {
                            a = *pMantissaReal;
                            b = *pMantissaImg;
                        }
                        else
                        {
                            a = -(*pMantissaReal);
                            b = -(*pMantissaImg);
                        }
                        *pMantissaReal = a * tmp.r - b * tmp.i;
                        *pMantissaImg = a * tmp.i + b * tmp.r;
                        if (piExponent)
                        {
                            if (C2F(pythag)(pMantissaReal, pMantissaImg) == 0.) /* original Fortran code does the fp strict compare */
                            {
                                break;
                            }
                            for (; C2F(pythag)(pMantissaReal, pMantissaImg) < 1 ; *pMantissaReal *= base, *pMantissaImg *= base, --(*piExponent))
                            {
                                ;
                            }
                            for (; C2F(pythag)(pMantissaReal, pMantissaImg) > base; *pMantissaReal /= base,  *pMantissaImg /= base, ++(*piExponent))
                            {
                                ;
                            }
                        }
                    }
                }
                else
                {
                    for (i = 0; i != iCols; ++i)
                    {
                        *pMantissaReal = ( (piPiv[i] == (i + 1)) ? *pMantissaReal : -(*pMantissaReal)) * pData[i * (iCols + 1)] ;
                        if (piExponent)
                        {
                            if (Abs(*pMantissaReal) == 0.) /* original Fortran code does the fp strict compare */
                            {
                                break;
                            }
                            for (; Abs(*pMantissaReal) < 1; *pMantissaReal *= base, --(*piExponent))
                            {
                                ;
                            }
                            for (; Abs(*pMantissaReal) >= base; *pMantissaReal /= base, ++(*piExponent))
                            {
                                ;
                            }
                        }
                    }
                }
            }
            FREE(piPiv);
        }
        else
        {
            info = -1;
        }
    }
    return info;
}
