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

#ifndef __TOSTRING_COMMON_HXX__
#define __TOSTRING_COMMON_HXX__

#include <sstream>
using namespace std;

#define SIZE_BETWEEN_TWO_VALUES			2
#define SPACE_BETWEEN_TWO_VALUES		"  "
#define SIZE_BETWEEN_REAL_COMPLEX		1
#define SPACE_BETWEEN_REAL_COMPLEX	" "
#define SIGN_LENGTH									2
#define NO_SIGN											"  "
#define MINUS_STRING								"- "
#define PLUS_STRING									"+ "
#define MINUS_STRING_INT						" -"
#define SYMBOL_I										"i"

#ifndef _MSC_VER
	#include <inttypes.h>
	#define _abs64(x) llabs(x)
#endif

/*double*/
void GetDoubleFormat(double _dblVal, int _iPrecNeeded, int *_piWidth, int *_piPrec, bool* _pbFloatingPoint);
void GetComplexFormat(double _dblR, double _dblI, int _iPrecNeeded, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR,  int *_piPrecI, bool* _pbFloatingPointR,  bool* _pbFloatingPointI);
void AddDoubleValue(ostringstream *_postr, double _dblVal, int _iWidth, int _iPrec, bool bPrintPlusSign = false, bool bPrintOne = true, bool bPaddSign = true);
void AddDoubleComplexValue(ostringstream *_postr, double _dblR, double _dblI, int _iTotalWitdh, int _iWidthR, int _iWidthI, int _iPrec);
void PrintDoubleVar(ostringstream *_postr,  double _dblVal);

/*Common*/
void Config_Stream(ostringstream *_postr, int _iWidth, int _iPrec, char _cFill);
void Add_Space(ostringstream *_postr, int _iSpace);

/*int*/
template <typename T>
void GetIntFormat(T _TVal, int *_piWidth)
{
	if(_TVal < 0)
	{
		*_piWidth = (int)(log10((long double)_abs64(_TVal)) + 1);
	}
	else
	{
		*_piWidth = (int)(log10((long double)_TVal) + 1);
	}
}

template <typename T>
void AddIntValue(ostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true)
{
	if(bPrintPlusSign == true)
	{
		*_postr << (_TVal < 0 ? MINUS_STRING_INT : PLUS_STRING);
	}
	else
	{
		*_postr << (_TVal < 0 ? MINUS_STRING_INT : NO_SIGN);
	}

	Config_Stream(_postr, _iWidth, 0, ' ');

	if(bPrintOne == true || _TVal != 1)
	{
		if(_TVal < 0)
		{
			*_postr << right << (long long)_abs64(_TVal);
		}
		else
		{
			*_postr << right << (unsigned long long)_TVal;
		}
	}
}


#endif /* __TOSTRING_COMMON_HXX__ */
