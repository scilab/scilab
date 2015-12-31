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
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "charEncoding.h"
#include "newest.h"
}

int newest(char** _filesString, int _iNbrOfFileString)
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
        char* FileName = NULL;

        FileName = expandPathVariable(_filesString[i]);

#ifdef _MSC_VER
        if (FileName)
        {
            if ( (FileName[strlen(FileName) - 1] == '/') || (FileName[strlen(FileName) - 1] == '\\') )
            {
                FileName[strlen(FileName) - 1] = '\0';
            }

        }
        resultstat = _stat(FileName, &buf);
#else
        resultstat = stat(FileName, &buf);
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
