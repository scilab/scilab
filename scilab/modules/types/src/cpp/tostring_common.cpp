/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <iostream>
#include <math.h>
#include "tostring_common.hxx"

extern "C"
{
	#include "elem_common.h"
}

using namespace std;

//template <typename T>
//void GetIntFormat(T _TVal, int *_piWidth)
//{
//	*_piWidth = (int)(log10((double)_abs64(_TVal)) + 1);
//}

//template <typename T>
//void AddIntValue(ostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign, bool bPrintOne)
//{
//	if(bPrintPlusSign == true)
//	{
//		*_postr << (_TVal < 0 ? MINUS_STRING_INT : PLUS_STRING);
//	}
//	else
//	{
//		*_postr << (_TVal < 0 ? MINUS_STRING_INT : NO_SIGN);
//	}
//
//	Config_Stream(_postr, _iWidth, 0, ' ');
//
//	if(bPrintOne == true || _TVal != 1)
//	{
//		*_postr << right << _abs64(_TVal);
//	}
//}

void GetDoubleFormat(double _dblVal, int _iPrecNeeded, int *_piWidth, int *_piPrec, bool* _pbFloatingPoint)
{

	double dblDec				= 0;
	double dblEnt				= 0;
	double dblAbs				= fabs(_dblVal);
	int iNbDigit				= 0;
	double dblTmp				= 0;
	int iNbEnt					= 0;
	int iNbDec					= 0;
	bool bStartByZero		= dblAbs < 1;

	dblDec				= modf(dblAbs, &dblEnt);
	*_pbFloatingPoint	= dblDec == 0 ? false : true;

	double dblTemp = log10(dblEnt + 0.4);
	iNbDigit = ((int)dblTemp) + 1;
	if(iNbDigit <= _iPrecNeeded)
	{
		iNbEnt		= iNbDigit;
		iNbDigit	= _iPrecNeeded - iNbEnt - 2;
		iNbDec		= 0;
		dblTmp		= dblDec * pow(10.0e+0, iNbDigit + 1);
		dblDec		= floor(dblTmp / 10.0e+0 + 0.5);

		if(dblDec != 0)
		{
			iNbDec = iNbDigit;
			while(fmod(dblDec, 10.0e+0) == 0)
			{
				iNbDec -= 1;
				dblDec = dblDec / 10.0e+0;
			}
		}
	}

	*_piPrec	= iNbDec + iNbEnt;
	*_piWidth	= *_piPrec > _iPrecNeeded ? _iPrecNeeded : *_piPrec;// + *_pbFloatingPoint;

}

void GetComplexFormat(double _dblR, double _dblI, int _iPrecNeeded, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR,  int *_piPrecI, bool* _pbFloatingPointR,  bool* _pbFloatingPointI)
{
	GetDoubleFormat(_dblR, _iPrecNeeded, _piWidthR, _piPrecR, _pbFloatingPointR);
	GetDoubleFormat(_dblI, _iPrecNeeded, _piWidthI, _piPrecI, _pbFloatingPointI);

	*_piTotalWidth = 0;
	if(_dblI == 0)
	{
		if(_dblR == 0)
		{
			*_piTotalWidth += 1; //"0"
		}
		else
		{
			*_piTotalWidth		+= *_piWidthR + *_pbFloatingPointR;
			*_piWidthI	= 0;
		}
	}
	else
	{
		if(_dblR == 0)
		{
			*_piTotalWidth		+= *_piWidthI + *_pbFloatingPointI;
			*_piWidthR	= 0;
		}
		else
		{
			*_piTotalWidth += *_piWidthR + *_pbFloatingPointR;
			//*_piTotalWidth += SIZE_BETWEEN_REAL_COMPLEX;
			*_piTotalWidth += SIGN_LENGTH;
			*_piTotalWidth += *_piWidthI + *_pbFloatingPointI;
		}
	}
}

void AddDoubleValue(wostringstream *_postr, double _dblVal, int _iWidth, int _iPrec, bool bPrintPlusSign, bool bPrintOne, bool bPaddSign)
{
	if(bPrintPlusSign == true)
	{
		*_postr << (_dblVal < 0 ? MINUS_STRING : PLUS_STRING);
	}
	else
	{
		if(bPaddSign)
		{
			*_postr << (_dblVal < 0 ? MINUS_STRING : NO_SIGN);
		}
		else
		{
			*_postr << (_dblVal < 0 ? MINUS_STRING : L"");
		}
	}

	Config_Stream(_postr, _iWidth/* + SIZE_BETWEEN_TWO_VALUES*/, _iPrec, ' ');

	if(bPrintOne == true || isEqual(_dblVal, 1) == false)
	{
		PrintDoubleVar(_postr, _dblVal);
	}
}

void AddDoubleComplexValue(wostringstream *_postr, double _dblR, double _dblI, int _iTotalWitdh, int _iWidthR, int _iWidthI, int _iPrec)
{
	wostringstream ostemp;
	/*
	if R && !C -> R
	if R && C -> R + Ci
	if !R && !C -> 0
	if(!R aa C	-> Ci
	*/

	int iSignLen = 0;
//	*_postr << "|%" << _iTotalWitdh << "%|";
	if(_dblR == 0)
	{//no real part
		if(_dblI == 0)
		{//no imaginary part

			//0
			ostemp << (_dblI < 0 ? MINUS_STRING : NO_SIGN);
			Config_Stream(&ostemp, _iWidthI, _iPrec, ' ');
			ostemp << left << 0;
			iSignLen = SIGN_LENGTH;
		}
		else
		{//imaginary part

			//I
			ostemp << (_dblI < 0 ? MINUS_STRING : NO_SIGN);
			Config_Stream(&ostemp, _iWidthI, _iPrec, ' ');
			if(fabs(_dblI) != 1)
			{//specail case if I == 1 write only i and not 1i
				PrintDoubleVar(&ostemp, _dblI);
			}
			ostemp << left << SYMBOL_I;
			iSignLen = SIGN_LENGTH;
		}
	}
	else
	{//real part
		if(_dblI == 0)
		{//no imaginary part

			//R
			ostemp << (_dblR < 0 ? MINUS_STRING : NO_SIGN);
			Config_Stream(&ostemp, _iWidthR, _iPrec, ' ');
			PrintDoubleVar(&ostemp, _dblR);
			iSignLen = SIGN_LENGTH;
		}
		else
		{//imaginary part

			//R
			ostemp << (_dblR < 0 ? MINUS_STRING : NO_SIGN);
			Config_Stream(&ostemp, _iWidthR, _iPrec, ' ');
			PrintDoubleVar(&ostemp, _dblR);
			ostemp << SPACE_BETWEEN_REAL_COMPLEX;

			//I
			ostemp << (_dblI < 0 ? MINUS_STRING : PLUS_STRING);
			Config_Stream(&ostemp, _iWidthI, _iPrec, ' ');
			if(fabs(_dblI) != 1)
			{//special case if I == 1 write only i and not 1i
				PrintDoubleVar(&ostemp, _dblI);
			}
			ostemp << left << SYMBOL_I;
			iSignLen = SIGN_LENGTH * 2;
		}
	}

	Config_Stream(_postr, _iTotalWitdh, _iPrec, ' ');
	*_postr << left << ostemp.str();

}

void PrintDoubleVar(wostringstream *_postr,  double _dblVal)
{
	if(ISNAN(_dblVal) == 1)
	{//NaN
		Config_Stream(_postr, 0, 0, ' ');
		*_postr << left << "Nan";
	}
	else if(finite(_dblVal))
	{//Normal case
		*_postr << left << fabs(_dblVal);
	}
	else
	{//Inf
		Config_Stream(_postr, 0, 0, ' ');
		*_postr << left << "Inf";
	}	

}
void Add_Space(wostringstream *_postr, int _iSpace)
{
	for(int i = 0 ; i < _iSpace ; i++)
	{
		*_postr << " ";
	}
}

void Config_Stream(wostringstream *_postr, int _iWidth, int _iPrec, char _cFill)
{
	_postr->width(_iWidth);
	_postr->precision(_iPrec);
	_postr->fill(_cFill);
}
