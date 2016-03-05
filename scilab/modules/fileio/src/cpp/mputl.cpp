/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "filemanager.hxx"
#include "sciprint.h"
#include <fcntl.h>

extern "C"
{
#include "mputl.h"
#include "sci_malloc.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
#define MPUTL_FORMAT "%s\n"
/*--------------------------------------------------------------------------*/
mputlError mputl(int _iFileId, wchar_t **pstStrings, int _iSizeStrings, BOOL _CR)
{
    int i = 0;
    types::File* pF = NULL;

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

            if ((pF->getFileModeAsInt() >= 100) && (pF->getFileModeAsInt() < 200) && ((pF->getFileModeAsInt() % 100) < 10) /* not r+ */)
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

