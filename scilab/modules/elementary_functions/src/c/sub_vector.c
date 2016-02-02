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

/*Substract two vectors*/
void vDless(int _iNbElem, double* _pdblIn1, double* _pdblIn2, int _iIncIn1, int _iIncIn2, double* _pdblOut)
{
    int iIndex1			= 0;
    double *pdblTemp	= NULL;

    pdblTemp = (double*)malloc(sizeof(double) * _iNbElem);
    for (iIndex1 = 0 ; iIndex1 < _iNbElem ; iIndex1++)
    {
        pdblTemp[iIndex1] = -_pdblIn2[iIndex1 * _iIncIn2];
    }

    vDadd(_iNbElem, _pdblIn1, pdblTemp, _iIncIn1, 1, _pdblOut);
    free(pdblTemp);
}

