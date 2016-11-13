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

/*Cumulative product*/
void vCupro(int _iNbElem, double* _piIn, double* _piOut)
{
    int iIndex = 0;
    double dblCoeff = 1;
    for (iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
    {
        _piOut[iIndex] = _piIn[iIndex] * dblCoeff;
        dblCoeff = _piOut[iIndex];
    }
}

/*Cumulative product complex*/
void vCuproi(int _iNbElem, double* _piRealIn, double* _piImgIn, double* _piRealOut, double* _piImgOut)
{
    int iIndex = 0;
    double dblRealCoeff = 1, dblImgCoeff = 0;
    for (iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
    {
        double dblTemp		= _piRealIn[iIndex];
        _piRealOut[iIndex]	= dblTemp * dblRealCoeff - dblImgCoeff * _piImgIn[iIndex];
        _piImgOut[iIndex]	= dblRealCoeff * _piImgIn[iIndex] + dblImgCoeff * dblTemp;
        dblRealCoeff		= _piRealOut[iIndex];
        dblImgCoeff			= _piImgOut[iIndex];
    }
}
