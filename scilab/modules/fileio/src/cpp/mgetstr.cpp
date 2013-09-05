/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include "filemanager.hxx"

extern "C"
{
#include "mgetstr.h"
#include "filesmanagement.h"
#include "MALLOC.h"
#include "charEncoding.h"
}

wchar_t* mgetstr(int _iFileId, int _iSizeToRead)
{
    wchar_t* pwstOut = NULL;
    File* pF = FileManager::getFile(_iFileId);

    if (pF != NULL)
    {
        if (pF->getFileModeAsInt() % 2 == 0) //to determine if the file have been opened with binary or text mode
        {
            int iSizeRead = 0;
            pwstOut = (wchar_t*)MALLOC((_iSizeToRead + 1) * sizeof(wchar_t));
            memset(pwstOut, 0x00, (_iSizeToRead + 1) * sizeof(wchar_t));

            while (_iSizeToRead > iSizeRead)
            {
                wchar_t* pwstRes = fgetws(&pwstOut[iSizeRead], _iSizeToRead - iSizeRead + 1, pF->getFiledesc());//fgetws need length to read + 1
                if (feof(pF->getFiledesc()))
                {
                    return pwstOut;
                }
                if (pwstRes == NULL)
                {
                    FREE(pwstOut);
                    return NULL;
                }
                iSizeRead += (int)wcslen(pwstRes);
            }
        }
        else
        {
            int iSizeRead = 0;
            char* buffer = (char*)MALLOC((_iSizeToRead + 1) * sizeof(char));
            memset(buffer, 0x00, (_iSizeToRead + 1) * sizeof(char));

            while (_iSizeToRead > iSizeRead)
            {
                char* pstRes = fgets(&buffer[iSizeRead], _iSizeToRead - iSizeRead + 1, pF->getFiledesc());//fgets need length to read + 1
                if (feof(pF->getFiledesc()))
                {
                    pwstOut = to_wide_string(buffer);
                    return pwstOut;
                }
                if (pstRes == NULL)
                {
                    FREE(buffer);
                    return NULL;
                }
                iSizeRead += (int)strlen(pstRes);
            }

            pwstOut = to_wide_string(buffer);
            FREE(buffer);
        }
    }

    return pwstOut;
}
