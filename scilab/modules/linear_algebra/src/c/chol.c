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
#include "chol.h"

void C2F(dpotrf)(char const uplo[1], int const* pN, double* pA, int const* pLd
                 , int const* pInfo);
void C2F(zpotrf)(char const uplo[1], int const* pN, doublecomplex* pA, int const* pLd
                 , int const* pInfo);


int iRealCholProduct(double *_pdblReal, int _iLeadDim)
{
    char cOrient = 'U';
    int iInfo;

    C2F(dpotrf)(&cOrient, &_iLeadDim, _pdblReal, &_iLeadDim, &iInfo);
    if (iInfo > 0)
    {
        return iInfo;
    }

    if (iInfo == 0 && _iLeadDim > 1)
    {
        int iIndex1 = 0, iIndex2 = 0;
        for (iIndex1 = 0 ; iIndex1 < _iLeadDim ; iIndex1++)
        {
            for (iIndex2 = iIndex1 + 1 ; iIndex2 < _iLeadDim ; iIndex2++)
            {
                _pdblReal[iIndex2 + iIndex1 * _iLeadDim] = 0;
            }
        }
    }

    return 0;
}

int iComplexCholProduct(doublecomplex *_poIn, int _iLeadDim)
{
    char cOrient = 'U';
    int iInfo;

    C2F(zpotrf)(&cOrient, &_iLeadDim, _poIn, &_iLeadDim, &iInfo);
    if (iInfo > 0)
    {
        return iInfo;
    }

    if (iInfo == 0 && _iLeadDim > 1)
    {
        int iIndex1 = 0, iIndex2 = 0;
        for (iIndex1 = 0 ; iIndex1 < _iLeadDim ; iIndex1++)
        {
            for (iIndex2 = iIndex1 + 1 ; iIndex2 < _iLeadDim ; iIndex2++)
            {
                _poIn[iIndex2 + iIndex1 * _iLeadDim].r = 0;
                _poIn[iIndex2 + iIndex1 * _iLeadDim].i = 0;
            }
        }
    }

    return 0;
}
