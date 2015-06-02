/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <cmath>
#include "types.hxx"
#include "double.hxx"
#include "string.hxx"
#include "scilab_sprintf.hxx"

using namespace types;

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_string.h"
#include "os_wtoi.h"
#include "os_string.h"
}

#define NanString L"Nan"
#define InfString L"Inf"
#define NegInfString L"-Inf"

static wchar_t* replaceAndCountLines(const wchar_t* _pwstInput, int* _piLines, int* _piNewLine);
static wchar_t* addl(TokenDef* token);
static void updatel(TokenDef* token);
wchar_t** scilab_sprintf(const char* _pstName, const wchar_t* _pwstInput, typed_list &in, ArgumentPosition* _pArgs, int _iArgsCount, int* _piOutputRows, int* _piNewLine)
{
    /* Force Windows display to have two-digit exponent. */
#ifdef _MSC_VER
    _set_output_format(_TWO_DIGIT_EXPONENT);
#endif
    wchar_t** pwstOutput = NULL;
    wchar_t* pwstFirstOutput = NULL;
    *_piNewLine = 0;
    size_t pos = 0;

    //\n \n\r \r \t to string
    //find number of lines
    // replace \\n \\t... by \n \t...
    pwstFirstOutput = replaceAndCountLines(_pwstInput, _piOutputRows, _piNewLine);

    //no arg, just return _pwstInput value
    if (_iArgsCount)
    {
        //store all sub parts of the input string
        //for the input string "bla1 %s bla2 %d bla3"
        //store  :
        //pwstToken[0] : "bla1 "
        //pwstToken[1] : "%s bla2 "
        //pwstToken[2] : "%d bla3"

        size_t iStart = 0;
        size_t iEnd = 0;
        int iToken = 0;
        int iPosArg = 0;

        TokenDef* pToken = new TokenDef[_iArgsCount + 1];
        wchar_t* pwstStart = pwstFirstOutput;

        bool bFinish = false;
        bool bPercentPercent = false;

        while (!bFinish)
        {
            wchar_t* pwstEnd = wcsstr(pwstStart + (iToken == 0 ? 0 : 1), L"%");
            iStart = pwstStart - pwstFirstOutput;
            bPercentPercent = false;
            if (pwstEnd != NULL)
            {
                if (iToken && pwstStart[1] == L'%')
                {
                    //manage "%%"
                    pwstEnd = wcsstr(pwstEnd + 1, L"%");
                    if (pwstEnd == NULL)
                    {
                        //end of string
                        iEnd = wcslen(pwstFirstOutput);
                        bFinish = true;
                    }
                    else
                    {
                        iEnd = pwstEnd - pwstFirstOutput;
                    }

                    // skip the first %
                    iStart++;
                    bPercentPercent = true;
                }
                else
                {
                    iEnd = pwstEnd - pwstFirstOutput;
                }
            }
            else
            {
                //end of string
                iEnd = wcslen(pwstFirstOutput);
                bFinish = true;
            }

            pToken[iToken].pwstToken = new wchar_t[iEnd - iStart + 1];
            wcsncpy(pToken[iToken].pwstToken, pwstFirstOutput + iStart, iEnd - iStart);
            pToken[iToken].pwstToken[iEnd - iStart] = L'\0';
            pToken[iToken].outputType = InternalType::ScilabNull;

            //identify destination type
            //format : %[flags][width][.precision][length]specifier
            //pToken[iToken].pwstToken

            //find %
            wchar_t* pwstPercent = wcsstr(pToken[iToken].pwstToken, L"%");
            if (pwstPercent != NULL && bPercentPercent == false)
            {
                //looking for flags
                if (*(pwstPercent + 1) == L'-' ||
                        *(pwstPercent + 1) == L'+' ||
                        *(pwstPercent + 1) == L' ' ||
                        *(pwstPercent + 1) == L'#' ||
                        *(pwstPercent + 1) == L'0')
                {
                    pwstPercent++;
                }

                //looking for width
                pToken[iToken].width = -1;
                if (*(pwstPercent + 1) == L'*')
                {
                    pwstPercent++;
                }
                else
                {
                    //number
                    if (iswdigit(*(pwstPercent + 1)))
                    {
                        pToken[iToken].width = os_wtoi(pwstPercent + 1);
                        while (iswdigit(*(pwstPercent + 1)))
                        {
                            pwstPercent++;
                        }
                    }
                }

                //looking for precision
                pToken[iToken].prec = -1;
                if (*(pwstPercent + 1) == L'.')
                {
                    pwstPercent++;

                    if (iswdigit(*(pwstPercent + 1)))
                    {
                        pToken[iToken].prec = os_wtoi(pwstPercent + 1);
                        while (iswdigit(*(pwstPercent + 1)))
                        {
                            pwstPercent++;
                        }
                    }
                }

                //looking for length
                if (*(pwstPercent + 1) == L'h' ||
                        *(pwstPercent + 1) == L'l' ||
                        *(pwstPercent + 1) == L'L')
                {
                    pToken[iToken].bLengthFlag = true;
                    pwstPercent++;
                }
                else
                {
                    pToken[iToken].bLengthFlag = false;
                }

                wchar_t wcType = *(pwstPercent + 1);
                pToken[iToken].typePos = (pwstPercent + 1) - pToken[iToken].pwstToken;
                switch (wcType)
                {
                    case L'i' : //integer
                    case L'd' : //integer
                        if (_pArgs[iPosArg].type != InternalType::ScilabDouble)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pstName);
                            *_piOutputRows = 0;
                            return NULL;
                        }

                        pToken[iToken].outputType = InternalType::ScilabInt32;

                        iPosArg++;
                        break;

                    case L'o' : //octal
                    case L'u' : //unsigned
                    case L'x' : //hex
                    case L'X' : //HEX
                        if (_pArgs[iPosArg].type != InternalType::ScilabDouble)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pstName);
                            *_piOutputRows = 0;
                            return NULL;
                        }

                        pToken[iToken].outputType = InternalType::ScilabUInt32;

                        iPosArg++;
                        break;
                    case L'f' : //float
                    case L'e' : //exp
                    case L'E' : //EXP
                    case L'g' : //shorter between float or exp
                    case L'G' : //shorter between float or EXP
                        if (_pArgs[iPosArg].type != InternalType::ScilabDouble)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pstName);
                            *_piOutputRows = 0;
                            return NULL;
                        }

                        pToken[iToken].outputType = InternalType::ScilabDouble;
                        iPosArg++;
                        break;
                    case L's' :
                    case L'c' :
                        if (_pArgs[iPosArg].type != InternalType::ScilabString)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pstName);
                            *_piOutputRows = 0;
                            return NULL;
                        }

                        if (pToken[iToken].bLengthFlag == false)
                        {
                            updatel(&(pToken[iToken]));
                        }

                        pToken[iToken].outputType = InternalType::ScilabString;
                        iPosArg++;
                        break;
                    default :
                        //houston ...
                        break;
                }
            }

            pwstStart = pwstEnd;
            iToken++;
        }

        FREE(pwstFirstOutput);
        pwstFirstOutput = NULL;

        int iLoop = 1;
        int iFirstArg = 1;
        if (strcmp(_pstName, "mfprintf") == 0)
        {
            iFirstArg = 2;
        }

        if (in.size() > 1)
        {
            iLoop = in[iFirstArg]->getAs<GenericType>()->getRows();
            for (int i = iFirstArg + 1 ; i < in.size() ; i++)
            {
                iLoop = std::min(iLoop, in[i]->getAs<GenericType>()->getRows());
            }
        }

        if (*_piNewLine || (*_piOutputRows) > 1)
        {
            (*_piOutputRows) *= iLoop;
        }

        std::wostringstream oFirstOutput;
        for (int j = 0 ; j < iLoop ; j++)
        {
            //copy the 0th token
            oFirstOutput << pToken[0].pwstToken;
            iPosArg = 0;
            //start at 1, the 0th is always without %
            for (int i = 1 ; i < _iArgsCount + 1 ; i++)
            {
                if (pToken[i].outputType == InternalType::ScilabDouble)
                {
                    wchar_t pwstTemp[bsiz];
                    double dblVal = in[_pArgs[iPosArg].iArg]->getAs<Double>()->get(j, _pArgs[iPosArg].iPos);

                    if (finite(dblVal))
                    {
                        os_swprintf(pwstTemp, bsiz, pToken[i].pwstToken, dblVal);
                    }
                    else
                    {
                        wchar_t* newToken = addl(&pToken[i]);

                        if (ISNAN(dblVal))
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, NanString);
                        }
                        else if (std::signbit(dblVal))
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, NegInfString);
                        }
                        else
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, InfString);
                        }

                        delete[] newToken;
                    }
                    iPosArg++;
                    oFirstOutput << pwstTemp;
                }
                else if (pToken[i].outputType == InternalType::ScilabInt32)
                {

                    wchar_t pwstTemp[bsiz];
                    double dblVal = in[_pArgs[iPosArg].iArg]->getAs<Double>()->get(j, _pArgs[iPosArg].iPos);

                    if (finite(dblVal))
                    {
                        os_swprintf(pwstTemp, bsiz, pToken[i].pwstToken, (int)dblVal);
                    }
                    else
                    {
                        wchar_t* newToken = addl(&pToken[i]);

                        if (ISNAN(dblVal))
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, NanString);
                        }
                        else
                        {
                            if (std::signbit(dblVal))
                            {
                                os_swprintf(pwstTemp, bsiz, newToken, NegInfString);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, newToken, InfString);
                            }
                        }

                        delete[] newToken;
                    }
                    iPosArg++;
                    oFirstOutput << pwstTemp;
                }
                else if (pToken[i].outputType == InternalType::ScilabUInt32)
                {

                    wchar_t pwstTemp[bsiz];
                    double dblVal = in[_pArgs[iPosArg].iArg]->getAs<Double>()->get(j, _pArgs[iPosArg].iPos);

                    if (finite(dblVal))
                    {
                        os_swprintf(pwstTemp, bsiz, pToken[i].pwstToken, (unsigned int)dblVal);
                    }
                    else
                    {
                        wchar_t* newToken = addl(&pToken[i]);

                        if (ISNAN(dblVal))
                        {
                            os_swprintf(pwstTemp, bsiz, newToken, NanString);
                        }
                        else
                        {
                            if (std::signbit(dblVal))
                            {
                                os_swprintf(pwstTemp, bsiz, newToken, NegInfString);
                            }
                            else
                            {
                                os_swprintf(pwstTemp, bsiz, newToken, InfString);
                            }
                        }

                        delete[] newToken;
                    }
                    iPosArg++;
                    oFirstOutput << pwstTemp;
                }
                else if (pToken[i].outputType == InternalType::ScilabString)
                {
                    wchar_t* pwstStr = NULL;

                    if (in[iPosArg + 1]->isDouble() && ISNAN(in[iPosArg + 1]->getAs<types::Double>()->get(0)))
                    {
                        pwstStr = NanString;
                    }
                    else if (in[iPosArg + 1]->isDouble() && finite(in[iPosArg + 1]->getAs<types::Double>()->get(0)) == false)
                    {
                        if (std::signbit(in[iPosArg + 1]->getAs<types::Double>()->get(0)))
                        {
                            pwstStr = NegInfString;
                        }
                        else
                        {
                            pwstStr = InfString;
                        }
                    }
                    else
                    {
                        pwstStr = in[_pArgs[iPosArg].iArg]->getAs<types::String>()->get(j, _pArgs[iPosArg].iPos);
                    }

                    int posC = (int)wcscspn(pToken[i].pwstToken, L"c");
                    int posS = (int)wcscspn(pToken[i].pwstToken, L"s");

                    if (posS == 0 || posC == 0)
                    {
                        *_piOutputRows = 0;
                        return NULL;
                    }

                    bool bC = posC < posS;
                    int len = 1;
                    if (pToken[i].prec == -1)
                    {
                        if (bC == false)
                        {
                            len = (int)wcslen(pwstStr);
                        }
                    }
                    else
                    {
                        if (bC == false)
                        {
                            len = std::min(pToken[i].prec, (int)wcslen(pwstStr));
                        }
                    }

                    int tokenLen = (int)wcslen(pToken[i].pwstToken);
                    len += tokenLen;
                    len = std::max(len, pToken[i].width);
                    //add len of string after token like "%20s>>>" add space for ">>>"
                    len += (tokenLen - (bC ? posC : posS));
                    wchar_t* pwstTemp = (wchar_t*)MALLOC((len + 1) * sizeof(wchar_t));

                    if (bC)
                    {
                        os_swprintf(pwstTemp, len + 1, pToken[i].pwstToken, pwstStr[0]);
                    }
                    else
                    {
                        os_swprintf(pwstTemp, len + 1, pToken[i].pwstToken, pwstStr);
                    }

                    oFirstOutput << pwstTemp;
                    FREE(pwstTemp);
                    iPosArg++;
                }
                else
                {
                    // management of %%
                    oFirstOutput << pToken[i].pwstToken;
                }
            }
        }

        pwstFirstOutput = os_wcsdup((wchar_t*)oFirstOutput.str().c_str());

        for (int j = 0; j < _iArgsCount + 1; ++j)
        {
            delete[] pToken[j].pwstToken;
        }
        delete[] pToken;

    }

    pwstOutput = (wchar_t**)MALLOC((*_piOutputRows) * sizeof(wchar_t*));

    size_t iLen = wcslen(pwstFirstOutput);
    int iStart = 0;
    int j = 0;
    for (int i = 0 ; i < iLen ; i++)
    {
        if (pwstFirstOutput[i] == L'\n')
        {
            int iSize = i - iStart;
            pwstOutput[j] = (wchar_t*)MALLOC(sizeof(wchar_t) * (iSize + 1));
            wcsncpy(pwstOutput[j], pwstFirstOutput + iStart, iSize);
            pwstOutput[j][iSize] = L'\0';
            iStart = i + 1;
            j++;
        }
    }

    if (j == (*_piOutputRows) - 1)
    {
        pwstOutput[j] = os_wcsdup(pwstFirstOutput + iStart);
    }

    FREE(pwstFirstOutput);
    return pwstOutput;
}
/*--------------------------------------------------------------------------*/
// replace "\\n" "\\r" "\\t" "\\r\\n" by '\n' '\r' '\t' '\n'
// count number of lines
// indicate if one '\n' is at the end of string
static wchar_t* replaceAndCountLines(const wchar_t* _pwstInput, int* _piLines, int* _piNewLine)
{
    size_t iInputLen = wcslen(_pwstInput);
    wchar_t* pwstFirstOutput = (wchar_t*)MALLOC(sizeof(wchar_t) * (iInputLen + 1));

    int iPos = 0;
    *_piLines = 1;

    for (int i = 0 ; i < iInputLen ; i++)
    {
        if (_pwstInput[i] == L'\\')
        {
            if (iInputLen == i + 1)
            {
                continue;
            }

            switch (_pwstInput[i + 1])
            {
                case L'n' :
                    pwstFirstOutput[iPos++] = L'\n';
                    (*_piLines)++;
                    i++;
                    break;
                case L'r' :
                    if (iInputLen > i + 3 && _pwstInput[i + 2] == L'\\' && _pwstInput[i + 3] == L'n')
                    {
                        pwstFirstOutput[iPos++] = L'\n';
                        (*_piLines)++;
                        i += 3;
                    }
                    else
                    {
                        pwstFirstOutput[iPos++] = L'\r';
                        i++;
                    }
                    break;
                case L't' :
                    pwstFirstOutput[iPos++] = L'\t';
                    i++;
                    break;
                case L'\\':
                    pwstFirstOutput[iPos++] = L'\\';
                    i++;
                    break;
                default:
                    break;
            }
        }
        else
        {
            pwstFirstOutput[iPos++] = _pwstInput[i];
        }
    }

    // do not count '\n' if it's at the end of string
    // it will be manage by piNewLine
    if (pwstFirstOutput[iPos - 1] == '\n')
    {
        (*_piLines)--;
        (*_piNewLine) = 1;
    }

    pwstFirstOutput[iPos] = 0;
    return pwstFirstOutput;
}
/*--------------------------------------------------------------------------*/
wchar_t* addl(TokenDef* token)
{
    //replace %s or %c by %ls or %lc to wide char compatibility
    int iPos = token->typePos;
    int sizeTotal = (int)wcslen(token->pwstToken);
    wchar_t* pwstToken = new wchar_t[sizeTotal + 2];

    wcsncpy(pwstToken, token->pwstToken, iPos);
    pwstToken[iPos] = L'l';
    pwstToken[iPos + 1] = L's';
    wcsncpy(&pwstToken[iPos + 2], token->pwstToken + iPos + 1, sizeTotal - (iPos + 1));
    pwstToken[sizeTotal + 1]  = L'\0';

    return pwstToken;
}
/*--------------------------------------------------------------------------*/
void updatel(TokenDef* token)
{
    wchar_t* newToken = addl(token);
    delete[] token->pwstToken;
    token->pwstToken = newToken;
}
