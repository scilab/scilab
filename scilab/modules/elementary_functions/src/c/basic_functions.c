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


/*absolute complex sum*/
double wasums(int _iNbElem, double* _pdblReal, double* _pdblImg)
{
	double dblRetVal = 0;
	int iIndex		= 0;
	int iOne		= 1;

	dblRetVal	= C2F(dasum)(&_iNbElem, _pdblReal, &iOne);
	dblRetVal	+= C2F(dasum)(&_iNbElem, _pdblImg, &iOne);

	return dblRetVal;
}


/*Cumulative sum*/
void vCusum(int _iNbElem, double *_pdblIn, double *_pdblOut)
{
	double dblTemp = 0;
	int iIndex = 0;

	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		_pdblOut[iIndex] = dblTemp + _pdblIn[iIndex];
		dblTemp = _pdblOut[iIndex];
	}
}


/*memset on vector*/
void vDset(int _iNbElem, double _dblVal, double* _pdblIn, int _iInc)
{
	int iIndex = 0;
	int iIndex2 = 0;
	if(_iInc < 0 )
		iIndex2 = (-_iNbElem+1) * _iInc + 1;

	for(iIndex = 0 ; iIndex < _iNbElem ; iIndex++)
	{
		_pdblIn[iIndex2] = _dblVal;
		iIndex2 += _iInc;
	}
}

/*frexp*/
double dfrexps(double _dblVal, double *_pdblExp)
{
	int iExp = 0;
	double dblCoef = 0;
	dblCoef = frexp(_dblVal, &iExp);
	*_pdblExp = iExp;
	return dblCoef;
}

double dblNearFloat(double _dblVal, double _dblMode)
{
    return C2F(nearfloat)(&_dblVal, &_dblMode);
}

double durands(int* _iVal)
{
	static int ia = 0, ic = 0, itwo = 2, m2 = 0, m = 0, mic = 0;
	static double halfm = 0, s = 0;

	if(m2 == 0)
	{
		m = 1;
		while(m > m2)
		{
			m2 = m;
			m = itwo * m2;
		}
		halfm = m2;

		ia = 8 * nint(halfm * datans(1) / 8) + 5;
		ic = 2 * nint(halfm * (0.5 - dsqrts(3) / 6)) + 1;
		mic = (m2 - ic) + m2;

		s = 0.5 / halfm;
	}

	*_iVal *= ia;

	if(*_iVal > mic)
		*_iVal = (*_iVal - m2) - m2;

	*_iVal += ic;

	if(*_iVal/2 > m2)
		*_iVal = (*_iVal - m2) - m2;

	if(*_iVal < 0)
		*_iVal = (*_iVal + m2) + m2;

	return (double)*_iVal * s;
}

int nint(double _iVal)
{
	return (int)(_iVal + 0.5);
}

/*aint return double round val */
double danints(double _dblVal)
{
	/* ATTENTION CECI N'EST PAS ENCORE COMPLETEMENT TESTE */
	/* 13/06/2008 ELAIS Antoine */

	double dblSign = dsigns(1, _dblVal);
	if(finite(dabss(_dblVal)) == 0)
	{//Infinite case !
		return _dblVal;
	}
	if(dblSign < 0)
		return ceil(_dblVal - 0.5);
	else
		return floor(_dblVal + 0.5);
	//return (double)(_dblVal + (0.5 * dblSign));
}




