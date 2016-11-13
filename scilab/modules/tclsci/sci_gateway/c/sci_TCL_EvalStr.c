/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
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
#include <stdio.h>
#include <string.h>

#include "gw_tclsci.h"
#include "TCL_Command.h"
#include "TCL_Global.h"
#include "Scierror.h"
#include "localization.h"
#include "GlobalTclInterp.h"
#include "os_string.h"
#include "api_scilab.h"
#include "sci_malloc.h"

/*--------------------------------------------------------------------------*/
int sci_TCL_EvalStr(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddrStr = NULL;
    int* piAddrl2 = NULL;
    char* l2 = NULL;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
    {
        char *tclSlave = NULL;
        char **Str = NULL;
        int m1, n1, i, j;
        int m2, n2;
        char **ReturnArrayString = NULL;
        int k = 0;
        int tclInterpReturnValue;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrStr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of string at position 1.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrStr, &m1, &n1, &Str))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 1);
            return 1;
        }

        if (!existsGlobalInterp())
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            freeAllocatedMatrixOfString(m1, n1, Str);
            return 0;
        }

        if (nbInputArgument(pvApiCtx) == 2)
        {
            // two arguments given - the slave interpreter name
            if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
            {
                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    freeAllocatedMatrixOfString(m1, n1, Str);
                    return 1;
                }

                // Retrieve a matrix of double at position 2.
                if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
                    freeAllocatedMatrixOfString(m1, n1, Str);
                    return 1;
                }

                if (!existsSlaveInterp((l2)))
                {
                    Scierror(999, _("%s: No such slave interpreter.\n"), fname);
                    freeAllocatedMatrixOfString(m1, n1, Str);
                    return 0;
                }
                tclSlave =  os_strdup((l2));
                freeAllocatedSingleString(l2);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
                freeAllocatedMatrixOfString(m1, n1, Str);
                return 0;
            }
        }

        ReturnArrayString = (char **) MALLOC(m1 * n1 * sizeof(char *));

        for (i = 0; i < m1 * n1 ; i++)
        {

            if (tclSlave != NULL)
            {
                tclInterpReturnValue = sendTclCommandToSlave(Str[i], tclSlave);
            }
            else
            {
                tclInterpReturnValue = sendTclCommand(Str[i]);
            }

            if (tclInterpReturnValue == TCL_ERROR)
            {
                const char *trace = NULL;

                // Read the error trace in the slave or in the main interpreter
                if (tclSlave != NULL)
                {
                    trace = Tcl_GetVar(Tcl_GetSlave(getTclInterp(), tclSlave), "errorInfo", TCL_GLOBAL_ONLY);
                }
                else
                {
                    trace = Tcl_GetVar(getTclInterp(), "errorInfo", TCL_GLOBAL_ONLY);
                }
                releaseTclInterp();
                for (j = 0; j < k; ++j)
                {
                    FREE(ReturnArrayString[j]);
                }
                FREE(ReturnArrayString);
                freeAllocatedMatrixOfString(m1, n1, Str);

                {
                    const char *result = NULL;

                    if (tclSlave != NULL) // In the slave
                    {
                        result = Tcl_GetStringResult(Tcl_GetSlave(getTclInterp(), tclSlave));
                    }
                    else // In the main interpreter
                    {
                        result = Tcl_GetStringResult(getTclInterp());
                    }
                    Scierror(999, "%s, %s at line %i\n	%s\n", fname, (char *)result, i + 1, (char *)trace);
                    releaseTclInterp();
                }
                FREE(tclSlave);
                return 0;
            }
            else
            {
                // return result of the successful evaluation of the script
                // return a matrix of string results
                ReturnArrayString[k++] = getTclCommandResult();
            }
        }

        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, ReturnArrayString);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            freeAllocatedMatrixOfString(m1, n1, ReturnArrayString);
            freeAllocatedMatrixOfString(m1, n1, Str);
            FREE(tclSlave);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

        freeAllocatedMatrixOfString(m1, n1, ReturnArrayString);
        freeAllocatedMatrixOfString(m1, n1, Str);
        FREE(tclSlave);

        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String or vector of strings expected.\n"), fname, 1);
        return 0;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
