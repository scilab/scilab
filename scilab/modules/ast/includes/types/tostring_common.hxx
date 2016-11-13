/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

#ifndef __TOSTRING_COMMON_HXX__
#define __TOSTRING_COMMON_HXX__

#include <sstream>
#include "os_string.h"
#include "dynlib_ast.h"

#define SIZE_BETWEEN_TWO_VALUES         1
#define SPACE_BETWEEN_TWO_VALUES        " "
#define SIZE_BOOL                       1
#define SIZE_BETWEEN_BOOL               1
#define SPACE_BETWEEN_BOOL              " "
#define SIGN_LENGTH                     1
#define NO_SIGN                         " "
#define MINUS_STRING                    "-"
#define PLUS_STRING                     "+"
#define SYMBOL_I                        "i"


#define MAX_LINES                       100
#ifndef _MSC_VER
#include <inttypes.h>
#define _abs64(x) llabs(x)
#endif

typedef struct __DOUBLE_FORMAT__
{
    __DOUBLE_FORMAT__() : iWidth(0), iPrec(0), bExp(false), bPrintPoint(true), bPrintPlusSign(false), bPrintOne(true), bPaddSign(true), iSignLen(SIGN_LENGTH), bPrintBlank(true), bPrintComplexPlusSpace(false) {}
    int iWidth;
    int iPrec;
    bool bExp;
    bool bPrintPoint;
    bool bPrintPlusSign;
    bool bPrintOne;
    bool bPaddSign;
    int iSignLen;
    bool bPrintBlank;
    bool bPrintComplexPlusSpace;
} DoubleFormat;

/*double*/
//void getDoubleFormat(double _dblVal, int *_piWidth, int *_piPrec, bool* _pExp);
EXTERN_AST void getDoubleFormat(double _dblVal, DoubleFormat* _pDF);
//void getComplexFormat(double _dblR, double _dblI, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR, int *_piPrecI, bool* _pExpR, bool* _pExpI);
EXTERN_AST void getComplexFormat(double _dblR, double _dblI, int *_piTotalWidth, DoubleFormat* _pDFR, DoubleFormat* _pDFI);

//addDoubleValue(int _iWidth, int _iPrec, bool _bExp, bool _bPrintPoint = true, bool _bPrintPlusSign = false, bool _bPrintOne = true, bool _bPaddSign = true, int _iSignLen = 2);
EXTERN_AST void addDoubleValue(std::ostringstream *_postr, double _dblVal, DoubleFormat* _pDF);
EXTERN_AST void addDoubleComplexValue(std::ostringstream *_postr, double _dblR, double _dblI, int _iTotalLen, DoubleFormat* _pDFR, DoubleFormat* _pDFI);

/*Common*/
EXTERN_AST void configureStream(std::ostringstream *_postr, int _iWidth, int _iPrec, char _cFill);
EXTERN_AST void addSpaces(std::ostringstream *_postr, int _iSpace);
void addColumnString(std::ostringstream& ostr, int _iFrom, int _iTo);
EXTERN_AST void printEmptyString(std::ostringstream& ostr);
EXTERN_AST void printDoubleValue(std::ostringstream& ostr, double val);
EXTERN_AST void printComplexValue(std::ostringstream& ostr, double val_r, double val_i);

/*int*/
/*
  We will specify all "signed" / "unsigned" cases to avoid Compilation Warning
*/

template <typename T>
void getUnsignedIntFormat(T _TVal, int *_piWidth)
{
    if (_TVal == 0)
    {
        *_piWidth = 0;
    }
    else
    {
        *_piWidth = static_cast<int>(log10(static_cast<long double>(_TVal)) + 1);
    }
    *_piWidth += 1;
}

template <typename T>
void getSignedIntFormat(T _TVal, int *_piWidth)
{
    if (_TVal == 0)
    {
        *_piWidth = 0;
    }
    else
    {
        unsigned long long a = _abs64(_TVal);
        long double b = static_cast<long double>(a);
        long double c = log10(b + 1);
        *_piWidth = static_cast<int>(c);
        //*_piWidth = static_cast<int>(log10(static_cast<unsigned long double>(_abs64(_TVal))) + 1);
    }
    *_piWidth += 1;
}

template <typename T>
void addUnsignedIntValue(std::ostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true)
{
    const char* pstSign = PLUS_STRING;
    char pstFormat[32];
    char pstOutput[32];
    if (bPrintPlusSign == true)
    {
        pstSign = PLUS_STRING;
    }
    else
    {
        pstSign = NO_SIGN;
    }

    if (bPrintOne == true || _TVal != 1)
    {
        os_sprintf(pstFormat, 32, " %s%llu", pstSign, (unsigned long long)(_TVal));
        os_sprintf(pstOutput, 32, "%*s", _iWidth + 1, pstFormat);//+1 for blank
        *_postr << pstOutput;
    }
}

template <typename T>
void addSignedIntValue(std::ostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign = false, bool bPrintOne = true)
{
    const char* pstSign = PLUS_STRING;
    char pstFormat[32];
    char pstOutput[32];
    if (bPrintPlusSign == true)
    {
        pstSign = (_TVal < 0 ? "-" : "+");
    }
    else
    {
        pstSign = (_TVal < 0 ? "-" : " ");
    }

    if (bPrintOne == true || _TVal != 1)
    {
        unsigned long long a = _abs64(_TVal);
        os_sprintf(pstFormat, 32, " %s%llu", pstSign, a);
        os_sprintf(pstOutput, 32, "%*s", _iWidth + 1, pstFormat);//+1 for blank
        *_postr << pstOutput;
    }
}


#endif /* __TOSTRING_COMMON_HXX__ */
