/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
* Copyright (C) 2006 - INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include "filemanager.hxx"

extern "C"
{
    //#include "PATH_MAX.h"
    //#include "gw_io.h"
#include "MALLOC.h"
    //#include "sciprint.h"
    //#include "stack-c.h"
#include "expandPathVariable.h"
    //#include "Scierror.h"
    //#include "localization.h"
    //#include "freeArrayOfString.h"
#include "charEncoding.h"
    //#include "os_strdup.h"
#include "newest.h"
}

int newest(wchar_t** _pwcsFilesString, int _iNbrOfFileString)
{
#ifdef _MSC_VER
    struct _stat buf;
#else
    struct stat buf;
#endif

    int i = 0;
    int RetVal = 0;

    int RetIndex = 1;
    long int MaxTime = 0;

    for (i = 0; i < _iNbrOfFileString ; i++)
    {

        int resultstat = 0;
        wchar_t *FileName = NULL;

        FileName = expandPathVariableW(_pwcsFilesString[i]);

#ifdef _MSC_VER
        if (FileName)
        {
            if ( (FileName[wcslen(FileName) - 1] == L'/') || (FileName[wcslen(FileName) - 1] == L'\\') )
            {
                FileName[wcslen(FileName) - 1] = L'\0';
            }

        }
        resultstat = _wstat(FileName, &buf );
#else
        char* temp = wide_string_to_UTF8(FileName);
        resultstat = stat(temp, &buf );
        FREE(temp);
#endif
        if (resultstat == 0)
        {
            if ((long int)buf.st_mtime > MaxTime)
            {
                MaxTime = (long int)buf.st_mtime;
                RetIndex = i + 1;
            }
        }

        FREE(FileName);
        FileName = NULL;
    }

    RetVal = RetIndex;
    return RetVal;
}
