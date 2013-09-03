/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "gw_history_manager.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_addhistory(char *fname, unsigned long fname_len)
{
    static int n1, m1;

    SciErr sciErr;
    int *piAddr1;
    int *piLen;
    int i;

    char **lines = NULL;
    BOOL bOK = FALSE;
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isStringType(pvApiCtx, piAddr1))
    {
        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        piLen = (int*)MALLOC(sizeof(int) * m1 * n1);

        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        lines = (char**)MALLOC(sizeof(char*) * m1 * n1);
        for (i = 0 ; i < m1 * n1 ; i++)
        {
            lines[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, piLen, lines);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }
        bOK = appendLinesToScilabHistory(lines, m1 * n1);
        freeArrayOfString(lines, m1 * n1);

        if (!bOK)
        {
            Scierror(999, _("%s: Could not add line to the history.\n"), fname, 1);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
