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

extern "C"
{
#include "MALLOC.h"
#include "os_wcsdup.h"
}

/*--------------------------------------------------------------------------*/
#define BLANK_CHARACTER L' '
#define TAB_CHARACTER L'\t'
/*--------------------------------------------------------------------------*/
static wchar_t* subwcs(const wchar_t *_pstStr, int _iStartPos, int _iEndPos);
/*--------------------------------------------------------------------------*/
String * stripblanks(String *InputStrings, bool bRemoveTAB)
{
    String *pOutputStrings = new String(InputStrings->getRows(), InputStrings->getCols());
    if (pOutputStrings)
    {
        pOutputStrings->set(InputStrings->get());

        for (int x = 0 ; x < InputStrings->getSize() ; x++)
        {
            wchar_t* pStr = InputStrings->get(x);
            int iInputStartIndex    = 0;
            int iInputEndIndex      = (int)wcslen(pStr);

            /* search character ' ' or TAB from end of the string */
            for (int i = static_cast<int>(wcslen(pStr) - 1) ; i >= 0 ; i--)
            {
                if (pStr[i] == BLANK_CHARACTER || (bRemoveTAB == true && pStr[i] == TAB_CHARACTER))
                {
                    iInputEndIndex--;
                }
                else
                {
                    break;
                }
            }

            /* search character ' ' or TAB from beginning of the string */
            for (int i = 0 ; i < static_cast<int>(wcslen(pStr)) ; i++)
            {
                if (pStr[i] == BLANK_CHARACTER || (bRemoveTAB == true && pStr[i] == TAB_CHARACTER))
                {
                    iInputStartIndex++;
                }
                else
                {
                    break;
                }
            }

            wchar_t* pwstReplace = subwcs(pStr, iInputStartIndex, iInputEndIndex);
            pOutputStrings->set(x, pwstReplace);
            FREE(pwstReplace);
        }
    }
    return pOutputStrings;
}
/*--------------------------------------------------------------------------*/
static wchar_t* subwcs(const wchar_t *_pstStr, int _iStartPos, int _iEndPos)
{
    int iLen = _iEndPos - _iStartPos;
    wchar_t* pwstBuf = NULL;

    //bad len or empty string
    if (iLen < 0 || wcscmp(_pstStr, L"") == 0)
    {
        return os_wcsdup(L"");
    }

    pwstBuf = (wchar_t*)MALLOC(sizeof(wchar_t) * (iLen + 1)); //+1 for null termination
    if (pwstBuf)
    {
        wcsncpy(pwstBuf, _pstStr + _iStartPos, iLen);   /*Put a part of str into stbuf*/
        pwstBuf[iLen] = L'\0';
    }
    return pwstBuf;
}
/*------------------------------------------------------------------------*/
