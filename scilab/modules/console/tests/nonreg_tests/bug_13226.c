/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2014 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <wchar.h>

#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"

extern void autoCompletionInConsoleMode(wchar_t ** commandLine, unsigned int *cursorLocation);

/*--------------------------------------------------------------------------*/
int sci_completecommandline(char *fname, unsigned long fname_len)
{
    SciErr err;
    int * addr = NULL;
    wchar_t * wstr = NULL;
    wchar_t * commandLine = MALLOC(sizeof(wchar_t) * 1024);
    double c;
    unsigned int cursor;

    CheckOutputArgument(pvApiCtx, 2, 2);
    CheckInputArgument(pvApiCtx, 2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    getAllocatedSingleWideString(pvApiCtx, addr, &wstr);
    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    getScalarDouble(pvApiCtx, addr, &c);

    cursor = (unsigned int)c;

    wcscpy(commandLine, wstr);
    freeAllocatedSingleWideString(wstr);

    autoCompletionInConsoleMode(&commandLine, &cursor);

    createSingleWideString(pvApiCtx, 3, commandLine);
    createScalarDouble(pvApiCtx, 4, (double)cursor);

    FREE(commandLine);

    AssignOutputVariable(pvApiCtx, 1) = 3;
    AssignOutputVariable(pvApiCtx, 2) = 4;

    return 0;
}
