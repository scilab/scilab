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

#include "types.hxx"
#include "double.hxx"
#include "string.hxx"
#include "scilab_sprintf.hxx"

using namespace types;

extern "C"
{
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_wcsdup.h"
}

static wchar_t* replaceAndCountLines(wchar_t* _pwstInput, int* _piLines, int* _piNewLine);

wchar_t** scilab_sprintf(wchar_t* _pwstName, wchar_t* _pwstInput, typed_list &in, ArgumentPosition* _pArgs, int _iArgsCount, int* _piOutputRows, int* _piNewLine)
{
    wchar_t** pwstOutput        = NULL;
    wchar_t* pwstFirstOutput    = NULL;
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

        size_t iStart   = 0;
        size_t iEnd     = 0;
        int iToken      = 0;
        int iPosArg     = 0;

        TokenDef* pToken = new TokenDef[_iArgsCount + 1];
        wchar_t* pwstStart  = pwstFirstOutput;

        bool bFinish         = false;
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
                        iEnd    = wcslen(pwstFirstOutput);
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
                iEnd    = wcslen(pwstFirstOutput);
                bFinish = true;
            }

            pToken[iToken].pwstToken = new wchar_t[iEnd - iStart + 1];
            wcsncpy(pToken[iToken].pwstToken, pwstFirstOutput + iStart, iEnd - iStart);
            pToken[iToken].pwstToken[iEnd - iStart] = L'\0';

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
                if (*(pwstPercent + 1) == L'*')
                {
                    pwstPercent++;
                }
                else
                {
                    //number
                    while (iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }

                //looking for precision
                if (*(pwstPercent + 1) == L'.')
                {
                    pwstPercent++;
                    while (iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
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
                switch (wcType)
                {
                    case L'i' : //integer
                    case L'd' : //integer
                    case L'o' : //octal
                    case L'u' : //unsigned
                    case L'x' : //hex
                    case L'X' : //HEX
                        if (_pArgs[iPosArg].type != InternalType::RealDouble)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                            return NULL;
                        }
                        pToken[iToken].outputType = InternalType::RealInt32;
                        iPosArg++;
                        break;
                    case L'f' : //float
                    case L'e' : //exp
                    case L'E' : //EXP
                    case L'g' : //shorter between float or exp
                    case L'G' : //shorter between float or EXP
                        if (_pArgs[iPosArg].type != InternalType::RealDouble)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                            return NULL;
                        }
                        pToken[iToken].outputType = InternalType::RealDouble;
                        iPosArg++;
                        break;
                    case L's' :
                    case L'c' :
                        if (_pArgs[iPosArg].type != InternalType::RealString)
                        {
                            Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                            return NULL;
                        }
                        pToken[iToken].outputType = InternalType::RealString;
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
        if (wcscmp(_pwstName, L"mfprintf") == 0)
        {
            iFirstArg = 2;
        }

        if (in.size() > 1)
        {
            iLoop = in[iFirstArg]->getAs<GenericType>()->getRows();
            for (int i = iFirstArg + 1 ; i < in.size() ; i++)
            {
                iLoop = Min(iLoop, in[i]->getAs<GenericType>()->getRows());
            }
        }

        if (*_piNewLine || (*_piOutputRows) > 1)
        {
            (*_piOutputRows) *= iLoop;
        }

        pwstFirstOutput = (wchar_t*)MALLOC(sizeof(wchar_t*) * iLoop * bsiz);
        memset(pwstFirstOutput, 0x00, sizeof(wchar_t*) * iLoop * bsiz);
        for (int j = 0 ; j < iLoop ; j++)
        {
            //copy the 0th token
            wcscat(pwstFirstOutput, pToken[0].pwstToken);
            iPosArg = 0;
            //start at 1, the 0th is always without %
            for (int i = 1 ; i < _iArgsCount + 1 ; i++)
            {
                wchar_t pwstTemp[bsiz];
                void* pvVal = NULL;
                if (pToken[i].outputType == InternalType::RealDouble)
                {
                    double dblVal = in[_pArgs[iPosArg].iArg]->getAs<Double>()->get(j, _pArgs[iPosArg].iPos);
                    swprintf(pwstTemp, bsiz, pToken[i].pwstToken, dblVal);
                    iPosArg++;
                }
                else if (pToken[i].outputType == InternalType::RealInt32)
                {
                    double dblVal = in[_pArgs[iPosArg].iArg]->getAs<Double>()->get(j, _pArgs[iPosArg].iPos);
                    swprintf(pwstTemp, bsiz, pToken[i].pwstToken, (int)dblVal);
                    iPosArg++;
                }
                else if (pToken[i].outputType == InternalType::RealString)
                {
                    wchar_t* pwstStr = in[_pArgs[iPosArg].iArg]->getAs<types::String>()->get(j, _pArgs[iPosArg].iPos);
                    int posC = (int)wcscspn(pToken[i].pwstToken, L"c");
                    int posS = (int)wcscspn(pToken[i].pwstToken, L"s");
                    if (!posS || !posC)
                    {
                        return NULL;
                    }
                    if (posC < posS)
                    {
                        if (pToken[i].bLengthFlag == false)
                        {
                            //replace %c by %lc to wide char compatibility
                            int sizeTotal = (int)wcslen(pToken[i].pwstToken);
                            wchar_t* pwstToken = (wchar_t*)MALLOC(sizeof(wchar_t) * (sizeTotal + 2));
                            wcsncpy(pwstToken, pToken[i].pwstToken, posC);
                            pwstToken[posC] = L'l';
                            wcsncpy(&pwstToken[posC + 1], &pToken[i].pwstToken[posC], sizeTotal - posC);
                            pwstToken[sizeTotal + 1]  = L'\0';
                            swprintf(pwstTemp, bsiz, pwstToken, pwstStr[0]);
                            FREE(pwstToken);
                        }
                        else
                        {
                            swprintf(pwstTemp, bsiz, pToken[i].pwstToken, pwstStr[0]);
                        }
                    }
                    else //'s'
                    {
                        if (pToken[i].bLengthFlag == false)
                        {
                            //replace %s by %ls to wide char compatibility
                            int sizeTotal = (int)wcslen(pToken[i].pwstToken);
                            wchar_t* pwstToken = (wchar_t*)MALLOC(sizeof(wchar_t) * (sizeTotal + 2));
                            wcsncpy(pwstToken, pToken[i].pwstToken, posS);
                            pwstToken[posS] = L'l';
                            wcsncpy(&pwstToken[posS + 1], &pToken[i].pwstToken[posS], sizeTotal - posS);
                            pwstToken[sizeTotal + 1]  = L'\0';
                            swprintf(pwstTemp, bsiz, pwstToken, pwstStr);
                            FREE(pwstToken);
                        }
                        else
                        {
                            swprintf(pwstTemp, bsiz, pToken[i].pwstToken, pwstStr);
                        }
                    }
                    iPosArg++;
                }
                else
                {
                    // management of %%
                    wcscpy(pwstTemp, pToken[i].pwstToken);
                }

                wcscat(pwstFirstOutput, pwstTemp);
            }
        }
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
static wchar_t* replaceAndCountLines(wchar_t* _pwstInput, int* _piLines, int* _piNewLine)
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
                pwstFirstOutput[iPos++] = L'\\';
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
                default:
                    pwstFirstOutput[iPos++] = L'\\';
                    i++;
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
