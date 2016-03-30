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

/*Add two vectors*/
void vDadd(int _iNbElem, double* _piIn1, double* _piIn2, int _iIncIn1, int _iIncIn2, double* _piOut)
{
    int iIndex = 0;
    if (_iIncIn1 == 1 && _iIncIn2 == 1)
    {
        for (iIndex = 0; iIndex < _iNbElem ; iIndex++)
        {
            _piOut[iIndex] = _piIn1[iIndex] + _piIn2[iIndex];
        }
    }
    else
    {
        int iIndex1 = 1;
        int iIndex2 = 1;
        if (_iIncIn1 < 0)
        {
            iIndex1 = (-_iNbElem + 1) * _iIncIn1 + 1;
        }
        if (_iIncIn2 < 0)
        {
            iIndex1 = (-_iNbElem + 1) * _iIncIn2 + 1;
        }

        for (iIndex = 0; iIndex < _iNbElem ; iIndex++)
        {
            _piOut[iIndex2] = _piIn1[iIndex1] + _piIn2[iIndex2];
            iIndex1			+= _iIncIn1;
            iIndex2			+= _iIncIn2;
        }
    }
}