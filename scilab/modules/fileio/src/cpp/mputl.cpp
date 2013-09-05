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
#include "MALLOC.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
#define MPUTL_FORMAT "%s\n"
/*--------------------------------------------------------------------------*/
mputlError mputl(int _iFileId, wchar_t **pstStrings, int _iSizeStrings, BOOL _CR)
{
    int i = 0;
    File* pF = NULL;

    if (pstStrings == NULL)
    {
        return MPUTL_ERROR;
    }

    if (_iFileId == STDIN_ID)
    {
        return MPUTL_INVALID_FILE_DESCRIPTOR;
    }
    else
    {
        pF = FileManager::getFile(_iFileId);
        if (pF)
        {
            // file opened with fortran open function
            if (pF->getFileType() == 1)
            {
                return MPUTL_ERROR;
            }

            if (pF->getFileModeAsInt() >= 100 && pF->getFileModeAsInt() < 200)
            {
                return MPUTL_NO_WRITE_RIGHT;
            }
        }
        else
        {
            return MPUTL_INVALID_FILE_DESCRIPTOR;
        }
    }

    for (i = 0; i < _iSizeStrings; i++)
    {
        char* pstTemp = NULL;
        pstTemp = wide_string_to_UTF8(pstStrings[i]);
        int iRet = fputs(pstTemp, pF->getFiledesc());
        FREE(pstTemp);
        if (iRet == -1)
        {
            return MPUTL_ERROR;
        }
        if (_CR)
        {
            iRet = fputs("\n", pF->getFiledesc());
            if (iRet == -1)
            {
                return MPUTL_ERROR;
            }
        }
    }

    return MPUTL_NO_ERROR;
}
/*--------------------------------------------------------------------------*/

