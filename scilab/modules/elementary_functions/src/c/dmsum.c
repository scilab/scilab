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

void ddmsums(int _iMode, double *_pdblIn, int _iRows, int _iCols, double *_pdblOut)
{
    int iIndex1		= 0;
    int iIndex2		= 0;

    switch (_iMode)
    {
        case BY_ALL :
            _pdblOut[0] = 0;
            for (iIndex1 = 0 ; iIndex1 < _iRows * _iCols ; iIndex1++)
            {
                _pdblOut[0] += _pdblIn[iIndex1];
            }
            break;
        case BY_ROWS :
            for (iIndex2 = 0 ; iIndex2 < _iCols ; iIndex2++)
            {
                _pdblOut[iIndex2] = 0;
                for (iIndex1 = 0 ; iIndex1 < _iRows ; iIndex1++)
                {
                    _pdblOut[iIndex2] += _pdblIn[iIndex2 * _iRows + iIndex1];
                }
            }
            break;
        case BY_COLS :
            for (iIndex2 = 0 ; iIndex2 < _iRows ; iIndex2++)
            {
                _pdblOut[iIndex2] = 0;
                for (iIndex1 = 0 ; iIndex1 < _iCols ; iIndex1++)
                {
                    _pdblOut[iIndex2] += _pdblIn[iIndex1 * _iRows + iIndex2];
                }
            }
            break;
    }
}

void zdmsums(int _iMode, double *_pdblRealIn, double *_pdblImgIn, int _iRows, int _iCols, double *_pdblRealOut, double *_pdblImgOut)
{
    ddmsums(_iMode, _pdblRealIn, _iRows, _iCols, _pdblRealOut);
    ddmsums(_iMode, _pdblImgIn, _iRows, _iCols, _pdblImgOut);
}
