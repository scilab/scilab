/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

template<typename T>
void vTransposeIntMatrix(T* _pIntIn, int _iRowsIn, int _iColsIn, T* _pIntOut)
{
    for (int iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
    {
        int iNewCoord = iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);
        _pIntOut[iNewCoord] = _pIntIn[iIndex];
    }
}
