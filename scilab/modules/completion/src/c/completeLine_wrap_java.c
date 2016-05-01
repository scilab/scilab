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
#include "completeLine_wrap_java.h"
#include "completeLine.h"
#include "getPartLine.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
char *completelineforjava(char *currentLine, char *stringToAdd, BOOL isFile, char *postCaretLine)
{
    char *assembledLine = NULL;
    char *patternFile = getFilePartLevel(currentLine);
    char *patternLine = getPartLevel(currentLine);

    assembledLine = completeLine(currentLine, stringToAdd, patternFile, patternLine, isFile, postCaretLine);
    if (patternFile)
    {
        FREE(patternFile);
        patternFile = NULL;
    }
    if (patternLine)
    {
        FREE(patternLine);
        patternLine = NULL;
    }

    return assembledLine;
}
/*--------------------------------------------------------------------------*/
