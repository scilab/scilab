/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>

#include "elem_common.h"

// converted a matrix of polynom to a polynomial matrix
double* dmp2pm(double** _pdblMP, int _iSizeMP, int* _piRanks, int _iMaxRank)
{
    int i           = 0;
    int iOne        = 1;
    int iSizePM     = 0;
    double* pdblPM  = 0;

    iSizePM = _iSizeMP * (_iMaxRank + 1);
    pdblPM = (double*)malloc(iSizePM * sizeof(double));
    memset(pdblPM, 0x00, iSizePM * sizeof(double));

    if(_piRanks == NULL && _iMaxRank == 0)
    {
        // shortcut in case where pdblMP come from a types::Double
        // a matrix of double considered as a polynomial of degree zero
        C2F(dcopy)(&_iSizeMP, _pdblMP[0], &iOne, pdblPM, &iOne);
    }
    else if(_iSizeMP == 1)
    {
        // shortcut in case where pdblMP is scalar polynom
        int iSize = _piRanks[0] + 1;
        C2F(dcopy)(&iSize, _pdblMP[0], &iOne, pdblPM, &iOne);
    }
    else
    {
        for(i = 0; i < _iSizeMP; i++)
        {
            int iSize = _piRanks[i] + 1;
            C2F(dcopy)(&iSize, _pdblMP[i], &iOne, pdblPM + i, &_iSizeMP);
        }
    }

    return pdblPM;
}
