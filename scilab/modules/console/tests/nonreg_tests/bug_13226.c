/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2014 - Calixte DENIZET
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

#include <wchar.h>

#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"

extern void autoCompletionInConsoleMode(wchar_t ** commandLine, unsigned int *cursorLocation);

/*--------------------------------------------------------------------------*/
int sci_completecommandline(char *fname, void *pvApiCtx)
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
