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


/*
etant donne un vecteur _piIn1 et un vecteur _piIn2,
 cette subroutine fait:
               _piOut = _piIn1 * _piIn2
quand les deux increments sont egaux a 1, cette
subroutine emploie des boucles "epanouis". dans le cas ou
les increments sont negatifs, cette subroutine prend
les composantes en ordre inverse.
*/
void vDvmul(int _iNbElem, double* _pdblIn1, double* _pdblIn2, int _iIncIn1, int _iIncIn2, double* _pdblOut)
{
	int iIndex = 0;
	if(_iIncIn1 == 1 && _iIncIn2 == 1)
	{
		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
			_pdblOut[iIndex] = _pdblIn1[iIndex] * _pdblIn2[iIndex];
	}
	else
	{
		int iIndex1 = 0;
		int iIndex2 = 0;
		if(_iIncIn1 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn1 + 1;
		if(_iIncIn2 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn2 + 1;

		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
		{
			_pdblOut[iIndex2] = _pdblIn1[iIndex1] * _pdblIn2[iIndex2];
			iIndex1			+= _iIncIn1;
			iIndex2			+= _iIncIn2;
		}
	}
}
/* vDvmul complex*/
void vWvmul(int _iNbElem, double* _piRealIn1, double* _piImgIn1, double* _piRealIn2, double* _piImgIn2, int _iIncIn1, int _iIncIn2, double* _piRealOut, double* _piImgOut)
{
	int iIndex = 0;
	if(_iIncIn1 == 1 && _iIncIn2 == 1)
	{
		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
		{
			double dblTemp		= _piRealIn2[iIndex];
			_piRealOut[iIndex]	= dblTemp * _piRealIn1[iIndex] - _piImgIn2[iIndex] * _piImgIn1[iIndex];
			_piImgOut[iIndex]	= dblTemp * _piImgIn1[iIndex] + _piImgIn2[iIndex] * _piRealIn1[iIndex];
		}
	}
	else
	{
		int iIndex1 = 0;
		int iIndex2 = 0;
		if(_iIncIn1 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn1 + 1;
		if(_iIncIn2 < 0)
			iIndex1 = (-_iNbElem + 1) * _iIncIn2 + 1;

		for(iIndex = 0; iIndex < _iNbElem ; iIndex++)
		{
			double dblTemp	= _piRealIn2[iIndex2];
			_piRealOut[iIndex2] = dblTemp * _piRealIn1[iIndex1] - _piImgIn2[iIndex2] * _piImgIn1[iIndex1];
			_piImgOut[iIndex2]	= dblTemp * _piImgIn1[iIndex1] + _piImgIn2[iIndex2] * _piRealIn1[iIndex1];
			iIndex1			+= _iIncIn1;
			iIndex2			+= _iIncIn2;
		}
	}
}
