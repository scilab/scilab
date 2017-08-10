/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
* Copyright (C) 2006 - INRIA - Allan CORNET
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
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include "filemanager.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "charEncoding.h"
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
