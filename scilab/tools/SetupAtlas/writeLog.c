/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
