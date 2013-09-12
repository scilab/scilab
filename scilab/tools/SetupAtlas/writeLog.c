/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <time.h>
#include <stdio.h>
#include "writeLog.h"
/*--------------------------------------------------------------------------*/
BOOL AppendMessageToLog(wchar_t *Message, wchar_t *FullFilename)
{
    BOOL bOK = FALSE;
    FILE *pFile = NULL;

    pFile = _wfopen(FullFilename, L"at");

    if (pFile)
    {
        fwprintf(pFile, L"%s\n", Message);
        fclose(pFile);
        bOK = TRUE;
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
wchar_t *getTimeString(void)
{
    wchar_t time_str[MAX_PATH];
    time_t timer;
    struct tm *currentTime;

    timer = time(NULL);
    currentTime = localtime(&timer);
    wsprintfW(time_str, L"%d/%d/%d %d:%d:%d",
              currentTime->tm_mon + 1 , currentTime->tm_mday, 1900 + currentTime->tm_year,
              currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);

    return _wcsdup(time_str);
}
/*--------------------------------------------------------------------------*/