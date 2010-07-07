/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Allan CORNET
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "stripblanks.hxx"
#include "os_wcsdup.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#define BLANK_CHARACTER L' '
#define TAB_CHARACTER L'\t'
/*--------------------------------------------------------------------------*/
static wchar_t* subwcs(const wchar_t *_pstStr, int _iStartPos, int _iEndPos);
/*--------------------------------------------------------------------------*/
String * stripblanks(String *InputStrings, bool bWithTAB)
{
    String *pOutputStrings = new String(InputStrings->rows_get(), InputStrings->cols_get());
    if(pOutputStrings)
    {
        pOutputStrings->string_set(InputStrings->string_get());

        for(int x = 0 ; x < InputStrings->size_get() ; x++)
        {
            wchar_t* pStr = InputStrings->string_get(x);
            int iInputStartIndex    = static_cast<int>(wcslen(pStr) - 1);
            int iInputEndIndex      = 0;

            /* search character ' ' or TAB from end of the string */
            for(int i = static_cast<int>(wcslen(pStr) - 1) ; i >= 0 ; i--)
            {
                if(pStr[i] != BLANK_CHARACTER || (bWithTAB && pStr[i] == TAB_CHARACTER))
                {
                    iInputEndIndex = i;
                    break;
                }
            }

            /* search character ' ' or TAB from beginning of the string */
            for(int i = 0 ; i < static_cast<int>(wcslen(pStr)) ; i++)
            {
                if(pStr[i] != BLANK_CHARACTER || (bWithTAB && pStr[i] == TAB_CHARACTER))
                {
                    iInputStartIndex = i;
                    break;
                }
            }

            if(iInputStartIndex <= iInputEndIndex)
            {
                /*Get the substring without tabs*/ 
                wchar_t* pstReplace = subwcs(pStr, iInputStartIndex, iInputEndIndex + 1 );
                /*To add the substring into the output matrix*/
                pOutputStrings->string_set(x, pstReplace);
                if(pstReplace)
                {
                    FREE(pstReplace);
                    pstReplace = NULL;
                }
            }
            else
            {//input string contains only BLANK or TAB characters
                pOutputStrings->string_set(x, L"");
            }
        }
    }
    return pOutputStrings;
}
/*--------------------------------------------------------------------------*/
static wchar_t* subwcs(const wchar_t *_pstStr, int _iStartPos, int _iEndPos)
{
    int iLen            = _iEndPos - _iStartPos;
    wchar_t* pstBuf    = NULL;

    //bad len or empty string
    if(iLen < 0 || wcscmp(_pstStr, L"") == 0)
    {
       return os_wcsdup(L"");
    }

    pstBuf = (wchar_t*)MALLOC(sizeof(wchar_t)*(iLen + 1)); //+1 for null termination
    if(pstBuf)
    {
        wcsncpy(pstBuf, _pstStr + _iStartPos, iLen);   /*Put a part of str into stbuf*/
        pstBuf[iLen] = 0;
    }
    return pstBuf;
}
/*------------------------------------------------------------------------*/
