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
#include "sci_malloc.h"
#include "charEncoding.h"
}

char* mgetstr(int _iFileId, int _iSizeToRead)
{
    char* pstOut = NULL;
    types::File* pF = FileManager::getFile(_iFileId);

    if (pF != NULL)
    {
        if (pF->getFileModeAsInt() % 2 == 0) //to determine if the file have been opened with binary or text mode
        {
            int iSizeRead = 0;
            pstOut = (char*)MALLOC((_iSizeToRead + 1) * sizeof(char));
            memset(pstOut, 0x00, (_iSizeToRead + 1) * sizeof(char));

            while (_iSizeToRead > iSizeRead)
            {
                char* pstRes = fgets(&pstOut[iSizeRead], _iSizeToRead - iSizeRead + 1, pF->getFiledesc());//fgetws need length to read + 1
                if (feof(pF->getFiledesc()))
                {
                    return pstOut;
                }
                if (pstRes == NULL)
                {
                    FREE(pstOut);
                    return NULL;
                }
                iSizeRead += (int)strlen(pstRes);
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
                    return buffer;
                }
                if (pstRes == NULL)
                {
                    FREE(buffer);
                    return NULL;
                }
                iSizeRead += (int)strlen(pstRes);
            }

            pstOut = buffer;
        }
    }

    return pstOut;
}
