/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "basic_functions.h"

/*Cumulative product*/
void vCupro(int _iNbElem, double* _piIn, double* _piOut)
{
	int iIndex = 0;
	double dblCoeff = 1;
	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
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
	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		double dblTemp		= _piRealIn[iIndex];
		_piRealOut[iIndex]	= dblTemp * dblRealCoeff - dblImgCoeff * _piImgIn[iIndex];
		_piImgOut[iIndex]	= dblRealCoeff * _piImgIn[iIndex] + dblImgCoeff * dblTemp;
		dblRealCoeff		= _piRealOut[iIndex];
		dblImgCoeff			= _piImgOut[iIndex];
	}
}
