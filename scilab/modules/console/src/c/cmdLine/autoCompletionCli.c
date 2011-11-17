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

void doCompletion(char *, int *cursor, int *cursor_max);

void autoCompletionInConsoleMode(wchar_t ** commandLine, unsigned int *cursorLocation);

void autoCompletionInConsoleMode(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    char *multiByteString = NULL;

    int nbrCharInString;

    multiByteString = wide_string_to_UTF8(*commandLine);
    nbrCharInString = wcslen(*commandLine);
    doCompletion(multiByteString, cursorLocation, &nbrCharInString);
    FREE(*commandLine);
    *commandLine = to_wide_string(multiByteString);
}
