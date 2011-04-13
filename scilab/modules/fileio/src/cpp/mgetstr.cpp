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
    int iSizeToRead = _iSizeToRead + 1; //fgetws and fgets need length to read + 1
    
    if(pF != NULL)
    {
        if(static_cast<int>(pF->getFileModeAsDouble()) % 2 == 1)
        {
            pwstOut = (wchar_t*)MALLOC(iSizeToRead * sizeof(wchar_t));
            memset(pwstOut, 0x00, iSizeToRead * sizeof(wchar_t));
            wchar_t* pwstRes = fgetws(pwstOut, iSizeToRead, pF->getFiledesc());
            if(pwstRes == NULL)
            {
                FREE(pwstOut);
                return NULL;
            }
        }
        else
        {
            char* buffer = (char*)MALLOC(iSizeToRead * sizeof(char));
            memset(buffer, 0x00, iSizeToRead * sizeof(char));

            char* pstRes = fgets(buffer, iSizeToRead, pF->getFiledesc());
            if(pstRes == NULL)
            {
                FREE(buffer);
                return NULL;
            }
            
            pwstOut = to_wide_string(buffer);
            FREE(buffer);
        }
    }
    else
    {
        return NULL;
    }
    
    return pwstOut;
}
