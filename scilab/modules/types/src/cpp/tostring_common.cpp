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

#include <wchar.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "tostring_common.hxx"
#include "formatmode.h"

extern "C"
{
#include "elem_common.h"
#include "os_swprintf.h"
}

#define BLANK_SIZE 1
#define POINT_SIZE 1
#define EXPOSANT_SIZE 2         //exposant symbol + exposant sign

using namespace std;

//template <typename T>
//void GetIntFormat(T _TVal, int *_piWidth)
//{
// *_piWidth = (int)(log10((double)_abs64(_TVal)) + 1);
//}

//template <typename T>
//void AddIntValue(ostringstream *_postr, T _TVal, int _iWidth, bool bPrintPlusSign, bool bPrintOne)
//{
// if(bPrintPlusSign == true)
// {
//  *_postr << (_TVal < 0 ? MINUS_STRING_INT : PLUS_STRING);
// }
// else
// {
//  *_postr << (_TVal < 0 ? MINUS_STRING_INT : NO_SIGN);
// }
//
// configureStream(_postr, _iWidth, 0, ' ');
//
// if(bPrintOne == true || _TVal != 1)
// {
//  *_postr << right << _abs64(_TVal);
// }
//}

void addSign(wostringstream * _postr, double _dblVal, bool _bPrintPlusSign, bool _bPaddSign)
{
    if (_bPrintPlusSign == true)
    {
        *_postr << (_dblVal < 0 ? MINUS_STRING : PLUS_STRING);
    }
    else
    {
        if (_bPaddSign)
        {
            *_postr << (_dblVal < 0 ? MINUS_STRING : NO_SIGN);
        }
        else
        {
            *_postr << (_dblVal < 0 ? MINUS_STRING : L"");
        }
    }
}

//void getDoubleFormat(double _dblVal, int *_piWidth, int *_piPrec, /*in/out*/bool* _pExp)
void getDoubleFormat(double _dblVal, DoubleFormat * _pDF)
{
    double dblDec = 0;
    double dblEnt = 0;
    double dblAbs = fabs(_dblVal);
    int iNbDigit = 0;
    int iNbDec = 0;

    _pDF->bExp |= getFormatMode() == 0;
    int iTotalLen = 0;
    int iPrecNeeded = getFormatSize();

    if (ISNAN(_dblVal) || !finite(_dblVal))
    {
        _pDF->iWidth = 5;      //" nan" or " inf"
        _pDF->iPrec = 0;
        return;
    }
    //get integer part and fractionnal part
    dblDec = modf(dblAbs, &dblEnt);

    //compute len of entire part
    if (dblEnt == 0)
    {
        //[-1, 1]
        iNbDigit = (int)fabs(floor(log10(dblAbs)));

        if (iNbDigit >= (iPrecNeeded - 2) || _pDF->bExp)
        {
            //exponant
            _pDF->bExp = true;
            iTotalLen = BLANK_SIZE + 1 /*integer before dot */  + POINT_SIZE + EXPOSANT_SIZE + (int)log10((double)iNbDigit) + 1;
            _pDF->iWidth = iPrecNeeded;
            _pDF->iPrec = iPrecNeeded - iTotalLen;
            return;
        }
        else
        {
            iTotalLen = BLANK_SIZE + 1 /*integer before dot */  + POINT_SIZE;
        }
    }
    else
    {
        double dblTemp = log10(dblEnt);

        if (dblTemp > (iPrecNeeded - 2) || _pDF->bExp)
        {
            //exponant mode
            if (dblTemp == 0)
            {
                dblTemp = 1;    //no incidence on value, just to allow log10(dblTemp)
            }
            _pDF->bExp = true;
            iTotalLen = BLANK_SIZE + 1 /*integer before dot */  + POINT_SIZE + EXPOSANT_SIZE + (int)log10(dblTemp) + 1;
            _pDF->iWidth = iPrecNeeded;
            _pDF->iPrec = iPrecNeeded - iTotalLen;
            return;
        }
        else
        {
            //number of digit in integer part
            iTotalLen = BLANK_SIZE + ((int)dblTemp + 1) + POINT_SIZE;
        }
    }

    //prepare fractionnal part to precision asked
    double dblScale = pow(10., iPrecNeeded - iTotalLen);
    while (iTotalLen <= iPrecNeeded)
    {
        dblDec = dblDec * dblScale;
        dblDec = floor(dblDec + 0.5);
        dblDec = dblDec / dblScale;

        // when dblAbs = 1.9999999..., modf function
        // return a decimal part equal to 1.0
        if (dblDec == 0. || dblDec == 1.)
        {
            break;
        }

        dblDec = dblDec * 10;
        int iFloor = (int)floor(dblDec);

        dblDec -= iFloor;
        iTotalLen++;
        iNbDec++;
        dblScale /= 10;
    }

    _pDF->iWidth = iTotalLen;
    _pDF->iPrec = iNbDec;
}

//void getComplexFormat(double _dblR, double _dblI, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR, int *_piPrecI, bool* _pExpR, bool* _pExpI)
void getComplexFormat(double _dblR, double _dblI, int *_piTotalWidth, DoubleFormat * _pDFR, DoubleFormat * _pDFI)
{
    getDoubleFormat(_dblR, _pDFR);
    getDoubleFormat(_dblI, _pDFI);

    *_piTotalWidth = 0;
    if (_dblI == 0)
    {
        if (_dblR == 0)
        {
            *_piTotalWidth += 1;    //"0"
        }
        else
        {
            *_piTotalWidth += _pDFR->iWidth;
            _pDFI->iWidth = 0;
        }
    }
    else
    {
        if (_dblR == 0)
        {
            *_piTotalWidth += _pDFI->iWidth;
            _pDFR->iWidth = 0;
        }
        else
        {
            *_piTotalWidth += _pDFR->iWidth + SIGN_LENGTH + _pDFI->iWidth;
        }
    }
}

void addDoubleValue(std::wostringstream * _postr, double _dblVal, DoubleFormat * _pDF)
{
    wchar_t pwstFormat[32];
    wchar_t pwstOutput[32];     // > @ format max
    wchar_t pwstSign[32];

    if (_pDF == NULL)
    {
        return;
    }

    if (_pDF->bPrintPlusSign)
    {
        os_swprintf(pwstSign, 32, L"%-*ls", _pDF->iSignLen, _dblVal < 0 ? MINUS_STRING : PLUS_STRING);
    }
    else
    {
        if (_pDF->bPaddSign)
        {
            os_swprintf(pwstSign, 32, L"%-*ls", _pDF->iSignLen, _dblVal < 0 ? MINUS_STRING : NO_SIGN);
        }
        else
        {
            os_swprintf(pwstSign, 32, L"%-*ls", _pDF->iSignLen, _dblVal < 0 ? MINUS_STRING : L"");
        }
    }

    if ((_pDF->bPrintOne == true) || (isEqual(fabs(_dblVal), 1)) == false)
    {
        //do not print if _bPrintOne == false && _dblVal == 1
        if (ISNAN(_dblVal))
        {
            //NaN
            os_swprintf(pwstOutput, 32, L"%ls%*ls", pwstSign, _pDF->iPrec, L"Nan");
        }
        else if (!finite(_dblVal))
        {
            //Inf
            os_swprintf(pwstOutput, 32, L"%ls%*ls", pwstSign, _pDF->iPrec, L"Inf");
        }
        else if (_pDF->bExp)
        {
            double dblAbs = fabs(_dblVal);
            double dblDec = 0;
            double dblEnt = 0;
            double dblTemp = 0;

            dblDec = modf(dblAbs, &dblEnt);
            if (dblEnt == 0)
            {
                dblTemp = floor(log10(dblDec));
            }
            else
            {
                dblTemp = log10(dblEnt);
            }
            dblDec = dblAbs / pow(10., (double)(int)dblTemp);
            dblDec = modf(dblDec, &dblEnt) * pow(10., _pDF->iPrec);

            if (_pDF->bPrintPoint)
            {
                os_swprintf(pwstFormat, 32, L"%ls%%#.0f%%0%d.0fD%%+d", pwstSign, _pDF->iPrec);
            }
            else
            {
                os_swprintf(pwstFormat, 32, L"%ls%%.0f%%0%d.0fD%%+d", pwstSign, _pDF->iPrec);
            }

            os_swprintf(pwstOutput, 32, pwstFormat, dblEnt, dblDec, (int)dblTemp);
        }
        else
        {
            if (_pDF->bPrintPoint)
            {
                os_swprintf(pwstFormat, 32, L"%ls%%#-%d.%df", pwstSign, _pDF->iWidth - 1, _pDF->iPrec);
            }
            else
            {
                os_swprintf(pwstFormat, 32, L"%ls%%-%d.%df", pwstSign, _pDF->iWidth - 2, _pDF->iPrec);  //-2 no point needed
            }

            os_swprintf(pwstOutput, 32, pwstFormat, fabs(_dblVal));
        }

        *_postr << pwstOutput;
    }
    else if (wcslen(pwstSign) != 0)
    {
        os_swprintf(pwstOutput, 32, L"%ls", pwstSign);
        *_postr << pwstOutput;
    }
}

/*
void addDoubleValue(wostringstream *_postr, double _dblVal, int _iWidth, int _iPrec, bool bPrintPlusSign, bool bPrintOne, bool bPaddSign)
{
    addSign(_postr, _dblVal, bPrintPlusSign, bPaddSign);
    configureStream(_postr, _iWidth, _iPrec, ' ');

    if(bPrintOne == true || isEqual(_dblVal, 1) == false)
    {
        NEWprintDoubleVar(_postr, _dblVal, _iWidth, _iPrec);
    }
}
*/
void addDoubleComplexValue(wostringstream * _postr, double _dblR, double _dblI, int _iTotalWidth, DoubleFormat * _pDFR, DoubleFormat * _pDFI)
{
    wostringstream ostemp;

    /*
     * if R && !C -> R
     * if R && C -> R + Ci
     * if !R && !C -> 0
     * if(!R && C -> Ci
     */

    // *_postr << "|%" << _iTotalWitdh << "%|";
    if (_dblR == 0)
    {
        //no real part
        if (_dblI == 0)
        {
            //no imaginary part

            //0
            DoubleFormat df;

            addDoubleValue(&ostemp, 0, &df);
        }
        else
        {
            //imaginary part

            //I
            DoubleFormat df;

            df.iWidth = 0;
            df.iPrec = _pDFI->iPrec;
            df.bExp = _pDFI->bExp;
            df.bPrintPlusSign = false;
            df.bPrintOne = false;
            addDoubleValue(&ostemp, _dblI, &df);
            ostemp << left << SYMBOL_I;
        }
    }
    else
    {
        //real part
        if (_dblI == 0)
        {
            //no imaginary part

            //R
            DoubleFormat df;

            df.iWidth = 0;
            df.iPrec = _pDFR->iPrec;
            df.bExp = _pDFR->bExp;
            addDoubleValue(&ostemp, _dblR, &df);
        }
        else
        {
            //imaginary part

            //R
            DoubleFormat df;

            df.iPrec = _pDFR->iPrec;
            df.bExp = _pDFR->bExp;

            addDoubleValue(&ostemp, _dblR, &df);
            ostemp << SPACE_BETWEEN_REAL_COMPLEX;

            //I
            df.iPrec = _pDFI->iPrec;
            df.bExp = _pDFI->bExp;
            df.bPrintPlusSign = true;
            df.bPrintOne = false;

            addDoubleValue(&ostemp, _dblI, &df);
            ostemp << left << SYMBOL_I;
        }
    }

    configureStream(_postr, _iTotalWidth, 0, ' ');
    *_postr << left << ostemp.str();
}

void addSpaces(wostringstream * _postr, int _iSpace)
{
    for (int i = 0; i < _iSpace; i++)
    {
        *_postr << L" ";
    }
}

void configureStream(wostringstream * _postr, int _iWidth, int _iPrec, char _cFill)
{
    _postr->setf(ios::showpoint);
    _postr->width(_iWidth);
    _postr->precision(_iPrec);
    _postr->fill(_cFill);
}
