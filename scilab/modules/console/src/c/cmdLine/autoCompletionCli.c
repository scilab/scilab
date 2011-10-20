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
#include "charEncoding.h"
#include "completion.h"
#include "charctl.h"

void autoCompletionInConsoleMode(wchar_t * commandLine, int *cursorLocation)
{
    char saveChar = 0;

    char *multiByteString = NULL;

    char **completedString = NULL;

    int sizeArray;

    saveChar = commandLine[*cursorLocation];
    commandLine[*cursorLocation] = '\0';
    multiByteString = wide_string_to_UTF8(commandLine);
    commandLine[*cursorLocation] = saveChar;
    completedString = completion(multiByteString, &sizeArray);
    while (sizeArray)
    {
        printf("%s\n", *completedString);
        completedString += 4;
        sizeArray--;
    }
}
