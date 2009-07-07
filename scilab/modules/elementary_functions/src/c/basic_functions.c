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
	double dblSignMode	= 0;
	double dblSignVal	= 0;
	double dblAbsVal	= 0;

	double dblRMax		= F2C(dlamch)("o",1L);
	double dblRMin		= F2C(dlamch)("u",1L);
	double dblBase		= F2C(dlamch)("b",1L);
	int iP				= (int)F2C(dlamch)("n",1L);
	double dblLogBase	= dlogs(dblBase);

	double dblUlp		= pow(dblBase, 1 - iP);
	double dblTiny		= 0;
	BOOL bNorm			= FALSE;

	if(dblRMin / dblRMax != 0)
	{
		int iIndex = 0;
		bNorm	= TRUE;
		dblTiny	= dblRMin;
		for(iIndex = 1 ; iIndex < iP ; iIndex++)
			dblTiny	/= dblBase;
	}
	else
		bNorm = FALSE;

	dblSignMode	= dsigns(1, _dblMode);
	dblSignVal	= dsigns(1, _dblVal);
	dblAbsVal	= dabss(_dblVal);

	if(ISNAN(_dblVal) == 1) // NaN
	{
		return _dblVal;
	}
	else if(dblAbsVal > dblRMax) // +/- Inf
	{
		if(dblSignMode * dblSignVal < 0)
			return dblSignVal * dblRMax;
		else
			return _dblVal;
	}
	else if(dblAbsVal >= dblRMin)
	{
		//usual case : x is a normalised floating point number
		// 1/ got the exponent e and the exponent part ep = base^e
		int iE			= (int)(dlogs(dblAbsVal) / dblLogBase);
		double dblEp	= pow(dblBase, iE);
		double dblMant	= 0;

		//in case of xa very near RMAX an error in e (of + 1)
		//result in an overflow in ep
		if(iE > dblRMax)
			iE		-= 1;
		//also in case of xa very near RMIN and when denormalised
		//number are not used, an error in e (of -1) results in a
		//flush to 0 for ep.
		if(dblEp == 0)
			iE		-= 1;

		dblEp	= pow(dblBase, iE);

		// 2/ got the mantissa
		dblMant	= dblAbsVal / dblEp;

		// 3/ verify that 1 <= m < BASE
		if(dblMant < 1)
		{
			while(dblMant < 1)
			{
				dblMant *= dblBase;
				dblEp	/= dblBase;
			}
		}
		else if(dblMant >= dblBase)
		{
			while(dblMant < 1)
			{
				dblMant /= dblBase;
				dblEp	*= dblBase;
			}
		}

		// 4/ now compute the near float
		if(dblSignMode * dblSignVal < 0)
		{
			if( dblMant == 1)
				return dblSignVal * (dblMant - (dblUlp / dblBase )) * dblEp;
			else
				return dblSignVal * (dblMant - dblUlp) * dblEp;
		}
		else
			return dblSignVal * (dblMant + dblUlp) * dblEp;
	}
	else if(_dblVal == 0)
	{//case x = 0  nearfloat depends if denormalised numbers are used
		if(bNorm)
			return dblSignMode * dblTiny;
		else
			return dblSignMode * dblRMin;
	}
	else //x is a denormalised number
		return _dblVal + dblSignMode * dblTiny;
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




