/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "charEncoding.h"
#include "completion.h"
#include "charctl.h"
#include "MALLOC.h"
#include "autoCompletionCli.h"

/* Autocopmpletion in NW/NWNI */
void autoCompletionInConsoleMode(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    char *multiByteString = NULL;

    wchar_t *wideString = NULL;

    int sizeToAlloc = 0;

    int nbrCharInString;

    multiByteString = wide_string_to_UTF8(*commandLine);
    nbrCharInString = wcslen(*commandLine);
    doCompletion(multiByteString, cursorLocation, &nbrCharInString);
    wideString = to_wide_string(multiByteString);
    /* Copy the new string in a buffer wich size is a multiple of 1024 */
    sizeToAlloc = 1024 * (wcslen(wideString) / 1024 + 1);
    FREE(*commandLine);
    *commandLine = MALLOC(sizeof(**commandLine) * sizeToAlloc);
    wcscpy(*commandLine, wideString);
    FREE(wideString);
    FREE(multiByteString);
}

/* Set new token in history in order to get string changement */
void updateTokenInScilabHistory(wchar_t ** commandLine)
{
    char *multiByteString = NULL;

    multiByteString = wide_string_to_UTF8(*commandLine);
    setSearchedTokenInScilabHistory(multiByteString);
    FREE(multiByteString);
}
