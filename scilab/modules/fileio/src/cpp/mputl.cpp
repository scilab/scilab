/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "filemanager.hxx"
#include "sciprint.h"
#include <fcntl.h>

extern "C"
{
#include "mputl.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
#define MPUTL_FORMAT "%s\n"
/*--------------------------------------------------------------------------*/
mputlError mputl(int _iFileId, wchar_t **pstStrings, int _iSizeStrings)
{
    int i = 0;
    File* pF = NULL;

    if(pstStrings == NULL)
    {
        return MPUTL_ERROR;
    }

    if(_iFileId == STDIN_ID)
    {
        return MPUTL_INVALID_FILE_DESCRIPTOR;
    }
    else
    {
        pF = FileManager::getFile(_iFileId);
        if(pF)
        {
            if(pF->getFileModeAsDouble() >= 100 && pF->getFileModeAsDouble() < 200)
            {
                return MPUTL_NO_WRITE_RIGHT;
            }
        }
        else
        {
            return MPUTL_INVALID_FILE_DESCRIPTOR;
        }
    }


    //export in UTF-8 if file is not open in binary mode
    if(static_cast<int>(pF->getFileModeAsDouble()) % 2 == 1)
    {//binary mode
        for (i = 0; i < _iSizeStrings; i++)
        {
            if(fputws(pstStrings[i], pF->getFiledesc()) == -1)
            {
                return MPUTL_ERROR;
            }

            if(fputws(L"\n", pF->getFiledesc()) == -1)
            {
                return MPUTL_ERROR;
            }
        }
    }
    else
    {//text mode
        for (i = 0; i < _iSizeStrings; i++)
        {
            char* pstTemp = wide_string_to_UTF8(pstStrings[i]);
            int iRet = fputs(pstTemp, pF->getFiledesc());
            FREE(pstTemp);
            if(iRet == -1)
            {
                return MPUTL_ERROR;
            }

            iRet = fputs("\n", pF->getFiledesc());
            if(iRet == -1)
            {
                return MPUTL_ERROR;
            }
        }
    }
    return MPUTL_NO_ERROR;
}
/*--------------------------------------------------------------------------*/ 

