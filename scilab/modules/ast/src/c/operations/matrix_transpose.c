/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "matrix_transpose.h"

/*
+---+---+
| 1 | 2 |       +---+---+---+
+---+---+       | 6 | 4 | 2 |
| 3 | 4 |   ->  +---+---+---+
+---+---+       | 5 | 3 | 1 |
| 5 | 6 |       +---+---+---+
+---+---+
*/
void vPretransposeRealMatrix(double *_pdblRealIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut)
{
    int iIndex = 0;
    for (iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
    {
        int Xa          = iIndex % _iRowsIn;
        int Ya          = iIndex / _iRowsIn;
        int Xb          = _iRowsIn - Xa - 1;
        int Yb          = _iColsIn - Ya - 1;
        int iNewCoord   = Xb * _iColsIn + Yb;

        _pdblRealOut[iNewCoord] = _pdblRealIn[iIndex];
    }
}

void vPretransposeComplexMatrix(double *_pdblRealIn, double *_pdblImgIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut, double *_pdblImgOut)
{
    int iIndex = 0;
    for (iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
    {
        int Xa          = iIndex % _iRowsIn;
        int Ya          = iIndex / _iRowsIn;
        int Xb          = _iRowsIn - Xa - 1;
        int Yb          = _iColsIn - Ya - 1;
        int iNewCoord   = Xb * _iColsIn + Yb;

        _pdblRealOut[iNewCoord] = _pdblRealIn[iIndex];
        _pdblImgOut[iNewCoord]  = -_pdblImgIn[iIndex];
    }
}

void vTransposeRealMatrix(double *_pdblRealIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut)
{
    int iIndex = 0;
    for (iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
    {
        /*  int iNewCoord = iIndex % _iColsIn * _iRowsIn + (iIndex / _iColsIn);
            _pdblRealOut[iIndex] = _pdblRealIn[iNewCoord];
        */
        int iNewCoord = iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);
        _pdblRealOut[iNewCoord] = _pdblRealIn[iIndex];
    }
}

void vTransposeComplexMatrix(double *_pdblRealIn, double *_pdblImgIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut, double *_pdblImgOut, int _iConjugate)
{
    int iIndex = 0;
    for (iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
    {
        int iNewCoord = iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);

        _pdblRealOut[iNewCoord] = _pdblRealIn[iIndex];
        if (_iConjugate == 0)
        {
            _pdblImgOut[iNewCoord] = _pdblImgIn[iIndex];
        }
        else
        {
            //Conjugate
            _pdblImgOut[iNewCoord] = -_pdblImgIn[iIndex];
        }
    }
}

void vTransposeDoubleComplexMatrix(doublecomplex *_poIn, int _iRowsIn, int _iColsIn, doublecomplex *_poOut, int _iConjugate)
{
    int iIndex = 0;
    for (iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
    {
        //int iNewCoord = iIndex % _iColsIn * _iRowsIn + (iIndex / _iColsIn);
        int iNewCoord = iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);
        _poOut[iNewCoord].r = _poIn[iIndex].r;
        if (_iConjugate == 0)
        {
            _poOut[iNewCoord].i = _poIn[iIndex].i;
        }
        else
        {
            //Conjugate
            _poOut[iNewCoord].i = -_poIn[iIndex].i;
        }
    }
}
