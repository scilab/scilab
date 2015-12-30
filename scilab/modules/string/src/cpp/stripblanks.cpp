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
#include "sci_malloc.h"
#include "os_string.h"
}

/*--------------------------------------------------------------------------*/
#define BLANK_CHARACTER L' '
#define TAB_CHARACTER L'\t'
/*--------------------------------------------------------------------------*/
static char* substr(const char* _pstStr, int _iStartPos, int _iEndPos);
/*--------------------------------------------------------------------------*/
types::String * stripblanks(types::String *InputStrings, bool bRemoveTAB)
{
    types::String *pOutputStrings = new types::String(InputStrings->getRows(), InputStrings->getCols());
    if (pOutputStrings)
    {
        pOutputStrings->set(InputStrings->get());

        for (int x = 0 ; x < InputStrings->getSize() ; x++)
        {
            char* pStr = InputStrings->get(x);
            int iInputStartIndex    = 0;
            int iInputEndIndex      = (int)strlen(pStr);

            /* search character ' ' or TAB from end of the string */
            for (int i = static_cast<int>(strlen(pStr) - 1) ; i >= 0 ; i--)
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
            for (int i = 0 ; i < static_cast<int>(strlen(pStr)) ; i++)
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

            char* pstReplace = substr(pStr, iInputStartIndex, iInputEndIndex);
            pOutputStrings->set(x, pstReplace);
            FREE(pstReplace);
        }
    }
    return pOutputStrings;
}
/*--------------------------------------------------------------------------*/
static char* substr(const char* _pstStr, int _iStartPos, int _iEndPos)
{
    int iLen = _iEndPos - _iStartPos;
    char* pstBuf = NULL;

    //bad len or empty string
    if (iLen < 0 || strcmp(_pstStr, "") == 0)
    {
        return os_strdup("");
    }

    pstBuf = (char*)MALLOC(sizeof(char) * (iLen + 1)); //+1 for null termination
    if (pstBuf)
    {
        strncpy(pstBuf, _pstStr + _iStartPos, iLen);   /*Put a part of str into stbuf*/
        pstBuf[iLen] = '\0';
    }
    return pstBuf;
}
/*------------------------------------------------------------------------*/
