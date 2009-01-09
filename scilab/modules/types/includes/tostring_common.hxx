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
#define SYMBOL_I										"i"

void GetFormat(double _dblVal, int _iPrecNeeded, int *_piWidth, int *_piPrec, bool* _pbFloatingPoint);
void GetComplexFormat(double _dblR, double _dblI, int _iPrecNeeded, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR,  int *_piPrecI, bool* _pbFloatingPointR,  bool* _pbFloatingPointI);
void Add_Value(ostringstream *_postr, double _dblVal, int _iWidth, int _iPrec, bool bPrintPlusSign = false, bool bPrintOne = true);
void Add_Complex_Value(ostringstream *_postr, double _dblR, double _dblI, int _iTotalWitdh, int _iWidthR, int _iWidthI, int _iPrec);
void Config_Stream(ostringstream *_postr, int _iWidth, int _iPrec, char _cFill);
void Add_Space(ostringstream *_postr, int _iSpace);

#endif /* __TOSTRING_COMMON_HXX__ */
