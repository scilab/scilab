/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
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
#include "basic_functions.h"
#include <string.h>

void franck_matrix(int _iSize, double *_pData)
{
    int iIndex1		= 0;
    int iIndex2		= 0;
    double dblVal	= _iSize;

    memset(_pData, 0x00, sizeof(double) * _iSize * _iSize);
    _pData[0]		= dblVal;

    if (_iSize == 1)
    {
        return;
    }

    for (iIndex1 = 1 ; iIndex1 < _iSize ; iIndex1++)
    {
        dblVal = _iSize - iIndex1;
        _pData[(iIndex1 - 1) * _iSize + iIndex1] = dblVal;
        for (iIndex2 = 0 ; iIndex2 <= iIndex1 ; iIndex2++)
        {
            _pData[iIndex1 * _iSize + iIndex2] = dblVal;
        }
    }
}

void hilb_matrix(int _iSize, double *_pData)
{
    int iIndex1		= 0;
    int iIndex2		= 0;
    double dblVal	= _iSize;
    double dblTemp	= 0;

    for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
    {
        if (iIndex1 != 0)
        {
            dblVal = ((_iSize - iIndex1) * dblVal * (_iSize + iIndex1)) / pow(iIndex1, 2);
        }
        dblTemp = dblVal * dblVal;

        _pData[iIndex1 * _iSize + iIndex1]	= dblTemp / ( 2 * iIndex1 + 1);
        if (iIndex1 == _iSize - 1)
        {
            break;
        }
        for (iIndex2 = iIndex1 + 1 ; iIndex2 < _iSize ; iIndex2++)
        {
            dblTemp = -((_iSize - iIndex2) * dblTemp * (_iSize + iIndex2)) / pow(iIndex2, 2);
            _pData[iIndex1 * _iSize + iIndex2] = dblTemp / (iIndex1 + iIndex2 + 1);
            _pData[iIndex2 * _iSize + iIndex1] = _pData[iIndex1 * _iSize + iIndex2];
        }
    }
}

void magic_matrix(int _iSize, double *_pData)
{
    int iNewSize	= 0;
    int iIndex1		= 0;
    int iIndex2		= 0;
    int iUn			= 1;
    int iTemp1		= 0;
    int iTemp2		= 0;

    if (_iSize % 4 != 0)
    {
        int iRow	= 0;
        int iCol	= 0;
        if (_iSize % 2 == 0)
        {
            iNewSize = _iSize / 2;
        }
        if (_iSize % 2 != 0)
        {
            iNewSize = _iSize;
        }

        //odd order or upper corner of even order

        iRow		= 0;
        iCol		= iNewSize / 2;
        memset(_pData, 0x00, sizeof(double) * _iSize * _iSize);
        for (iIndex1 = 0 ; iIndex1 < iNewSize * iNewSize ; iIndex1++)
        {
            int iRowTemp	= 0;
            int iColTemp	= 0;

            _pData[iRow + iCol * _iSize] = iIndex1 + 1;
            iRowTemp		= iRow - 1;
            iColTemp		= iCol + 1;

            if (iRowTemp < 0)
            {
                iRowTemp	= iNewSize - 1;
            }
            if (iColTemp >= iNewSize)
            {
                iColTemp	= 0;
            }

            if (_pData[iRowTemp + iColTemp * _iSize] != 0)
            {
                iRowTemp	= iRow + 1;
                iColTemp	= iCol;
            }
            iRow		= iRowTemp;
            iCol		= iColTemp;
        }
        if (_iSize % 2 != 0)
        {
            return;
        }

        //rest of even order
        for (iIndex1 = 0 ; iIndex1 < iNewSize ; iIndex1++)
        {
            for (iIndex2 = 0 ; iIndex2 < iNewSize ; iIndex2++)
            {
                int iRow = iIndex1 + iNewSize;
                int iCol = iIndex2 + iNewSize;

                _pData[iIndex1 + iCol * _iSize] = _pData[iIndex1 + iIndex2 * _iSize] + 2 * iNewSize * iNewSize;
                _pData[iRow + iIndex2 * _iSize] = _pData[iIndex1 + iIndex2 * _iSize] + 3 * iNewSize * iNewSize;
                _pData[iRow + iCol * _iSize]	= _pData[iIndex1 + iIndex2 * _iSize] + iNewSize * iNewSize;
            }
        }
        if ((iNewSize - 1) / 2 == 0)
        {
            return;
        }
        for (iIndex1 = 0 ; iIndex1 < (iNewSize - 1) / 2 ; iIndex1++)
        {
            C2F(dswap)(&iNewSize, &_pData[iIndex1 * _iSize], &iUn, &_pData[iNewSize + iIndex1 * _iSize], &iUn);
        }

        iTemp1	= (iNewSize + 1) / 2 - 1;
        iTemp2	= iTemp1 + iNewSize;
        C2F(dswap)(&iUn, &_pData[iTemp1], &iUn, &_pData[iTemp2], &iUn);
        C2F(dswap)(&iUn, &_pData[iTemp1 * _iSize + iTemp1], &iUn, &_pData[iTemp1 * _iSize + iTemp2], &iUn);
        iTemp1 = _iSize - (iNewSize - 3) / 2;
        if (iTemp1 > _iSize)
        {
            return;
        }
        for (iIndex1 = iTemp1 ; iIndex1 < _iSize ; iIndex1++)
        {
            C2F(dswap)(&iNewSize, &_pData[iIndex1 * _iSize], &iUn, &_pData[iNewSize + iIndex1 * _iSize], &iUn);
        }
    }
    else
    {
        int iVal = 1;
        for (iIndex1 = 0 ; iIndex1 < _iSize ; iIndex1++)
        {
            for (iIndex2 = 0 ; iIndex2 < _iSize ; iIndex2++)
            {
                _pData[iIndex2 * _iSize + iIndex1] = iVal;
                if (((iIndex1 + 1) % 4) / 2 == ((iIndex2 + 1) % 4) / 2)
                {
                    _pData[iIndex2 * _iSize + iIndex1] = _iSize * _iSize + 1 - iVal;
                }
                iVal++;
            }
        }
    }
}
