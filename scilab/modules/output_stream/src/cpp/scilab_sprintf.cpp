/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

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

wchar_t** scilab_sprintf(wchar_t* _pwstName, wchar_t* _pwstInput, typed_list &in, ArgumentPosition* _pArgs, int _iArgsCount, int* _piOutputRows)
{
    wchar_t** pwstOutput        = NULL;
    wchar_t* pwstFirstOutput    = NULL;
    
    if(_iArgsCount == 0)
    {//no arg, just return _pwstInput value
        pwstFirstOutput = os_wcsdup(_pwstInput);
    }
    else
    {
        //store all sub parts of the input string
        //for the input string "bla1 %s bla2 %d bla3"
        //store  :
        //pwstToken[0] : "bla1 "
        //pwstToken[0] : "%s bla2 "
        //pwstToken[2] : "%d bla3"

        size_t iStart       = 0;
        size_t iEnd         = 0;
        int iToken          = 0;
        TokenDef* pToken = new TokenDef[_iArgsCount + 1];
        wchar_t* pwstStart  = _pwstInput;

        bool bFinish = false;
        while(!bFinish)
        {
            wchar_t* pwstEnd = wcsstr(pwstStart + (iToken == 0 ? 0 : 1), L"%");
            if(pwstEnd != NULL)
            {
                if(pwstEnd[1] == L'%')
                {//manage "%%"
                    pwstEnd = wcsstr(pwstEnd + 2, L"%");
                    if(pwstEnd == NULL)
                    {//end of string
                        iStart  = pwstStart - _pwstInput;
                        iEnd = wcslen(_pwstInput);
                        bFinish = true;
                    }
                }
                else
                {
                    iStart  = pwstStart - _pwstInput;
                    iEnd    = pwstEnd - _pwstInput;
                }
            }
            else
            {//end of string
                iStart  = pwstStart - _pwstInput;
                iEnd = wcslen(_pwstInput);
                bFinish = true;
            }

            pToken[iToken].pwstToken = new wchar_t[iEnd - iStart + 1];

            wcsncpy(pToken[iToken].pwstToken, _pwstInput + iStart, iEnd - iStart);
            pToken[iToken].pwstToken[iEnd - iStart] = L'\0';

            //identify destination type
            //format : %[flags][width][.precision][length]specifier
            //pToken[iToken].pwstToken

            //find %
            wchar_t* pwstPercent = wcsstr(pToken[iToken].pwstToken, L"%");

            if(pwstPercent != NULL)
            {
                //looking for flags
                if(*(pwstPercent + 1) == L'-' || 
                    *(pwstPercent + 1) == L'+' || 
                    *(pwstPercent + 1) == L' ' || 
                    *(pwstPercent + 1) == L'#' || 
                    *(pwstPercent + 1) == L'0')
                {
                    pwstPercent++;
                }

                //looking for width
                if(*(pwstPercent + 1) == L'*')
                {
                    pwstPercent++;
                }
                else
                {//number
                    while(iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }

                //looking for precision
                if(*(pwstPercent + 1) == L'.')
                {
                    pwstPercent++;
                    while(iswdigit(*(pwstPercent + 1)))
                    {
                        pwstPercent++;
                    }
                }

                //looking for length
                if(*(pwstPercent + 1) == L'h' || 
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
                switch(wcType)
                {
                case L'i' : //integer
                case L'd' : //integer
                case L'o' : //octal
                case L'u' : //unsigned
                case L'x' : //hex
                case L'X' : //HEX
                    if(_pArgs[iToken - 1].type != InternalType::RealDouble)
                    {
                        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                        return NULL;
                    }
                    pToken[iToken].outputType = InternalType::RealInt;
                    break;
                case L'f' : //float
                case L'e' : //exp
                case L'E' : //EXP
                case L'g' : //shorter between float or exp
                case L'G' : //shorter between float or EXP
                    if(_pArgs[iToken - 1].type != InternalType::RealDouble)
                    {
                        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                        return NULL;
                    }
                    pToken[iToken].outputType = InternalType::RealDouble;
                    break;
                case L's' :
                case L'c' :
                    if(_pArgs[iToken - 1].type != InternalType::RealString)
                    {
                        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), _pwstName);
                        return NULL;
                    }
                    pToken[iToken].outputType = InternalType::RealString;
                    break;
                default : 
                    //houston ...
                    break;
                }

            }

            pwstStart = pwstEnd;
            iToken++;
        }

        int iLoop = in[1]->getAsGenericType()->rows_get();
        pwstFirstOutput = (wchar_t*)MALLOC(sizeof(wchar_t*) * iLoop * bsiz);
        memset(pwstFirstOutput, 0x00, sizeof(wchar_t*) * iLoop * bsiz);
        for(int j = 0 ; j < iLoop ; j++)
        {
            //copy the 0th token
            wcscat(pwstFirstOutput, pToken[0].pwstToken);

            //start at 1, the 0th is always without %
            for(int i = 1 ; i < _iArgsCount + 1 ; i++)
            {
                wchar_t pwstTemp[bsiz];
                void* pvVal = NULL;
                if(_pArgs[i - 1].type == InternalType::RealDouble)
                {
                    double dblVal = in[_pArgs[i - 1].iArg]->getAsDouble()->real_get(j, _pArgs[i - 1].iPos);
                    if(pToken[i].outputType == InternalType::RealDouble)
                    {
                        swprintf(pwstTemp, bsiz, pToken[i].pwstToken, dblVal);
                    }
                    else if(pToken[i].outputType == InternalType::RealInt)
                    {
                        swprintf(pwstTemp, bsiz, pToken[i].pwstToken, (int)dblVal);
                    }
                }
                else if(_pArgs[i - 1].type == InternalType::RealString)
                {
                    wchar_t* pwstStr = in[_pArgs[i - 1].iArg]->getAsString()->string_get(j, _pArgs[i - 1].iPos);

#ifdef _MSC_VER
                    swprintf(pwstTemp, bsiz, pToken[i].pwstToken, pwstStr);
#else
                    if(pToken[i].bLengthFlag == false)
                    {
                        //replace %s by %ls to wide char compatibility
                        wchar_t* pwstToken = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(pToken[i].pwstToken) + 2));
                        swprintf(pwstToken, wcslen(pToken[i].pwstToken) + 2, pToken[i].pwstToken, "%ls");
                        swprintf(pwstTemp, bsiz, pwstToken, pwstStr);
                        FREE(pwstToken);
                    }
                    else
                    {
                        swprintf(pwstTemp, bsiz, pToken[i].pwstToken, pwstStr);
                    }
#endif
                }
                else
                {//impossible but maybe in the futur
                }

                wcscat(pwstFirstOutput, pwstTemp);
            }
        }
    }

    //\n \n\r \r to string
    //find number of lines
    *_piOutputRows = 1;
    int iLen = wcslen(pwstFirstOutput) - 1;
    for(int i = 0 ; i < iLen ; i++)
    {
        if(pwstFirstOutput[i] == L'\\' && pwstFirstOutput[i + 1] == L'r' && pwstFirstOutput[i + 2] != L'\0')
        {
            (*_piOutputRows)++;
            i += 1;
            if(pwstFirstOutput[i] == L'\\' && pwstFirstOutput[i + 1] == L'n' && pwstFirstOutput[i + 2] != L'\0')
            {
                i += 2;
            }
        }
        else if(pwstFirstOutput[i] == L'\\' && pwstFirstOutput[i + 1] == L'n' && pwstFirstOutput[i + 2] != L'\0')
        {
            (*_piOutputRows)++;
            i += 1;
        }
    }

    //alloc output data
    pwstOutput = (wchar_t**)MALLOC(sizeof(wchar_t*) * *_piOutputRows);
    wchar_t* pwstPtr = pwstFirstOutput;
    int iRows = 0;

    //split in multiple strings
    for(int i = 0 ; i < wcslen(pwstPtr) ; i++)
    {
        int idx = 0;
        bool bNewLine = false;
        if(pwstPtr[i] == L'\\' && pwstPtr[i + 1] == L'r')
        {
            idx = i;
            bNewLine = true;
            i += 2;
            if(pwstPtr[i] == L'\\' && pwstPtr[i + 1] == L'n')
            {
                i += 2;
            }
        }
        else if(pwstPtr[i] == L'\\' && pwstPtr[i + 1] == L'n')
        {
            idx = i;
            bNewLine = true;
            i += 2;
        }

        if(bNewLine || pwstPtr[i + 1] == L'\0')
        {
            if(pwstPtr[i + 1] == L'\0' && !bNewLine)
            {//to copy end of data in a new lines
                idx = wcslen(pwstPtr);
                i = idx ;
            }
            else if(bNewLine)
            {//to insert '\0'
                idx++;
            }

            pwstOutput[iRows] = (wchar_t*)MALLOC(sizeof(wchar_t) * (idx + 1));
            wcsncpy(pwstOutput[iRows], pwstPtr, idx);

            if(bNewLine)
            {
                pwstOutput[iRows][idx - 1] = L'\n';
            }

            pwstOutput[iRows][idx] = L'\0';
            pwstPtr += i;
            iRows++;
            i = -1;
        }
    }

    return pwstOutput;
}
/*--------------------------------------------------------------------------*/ 
