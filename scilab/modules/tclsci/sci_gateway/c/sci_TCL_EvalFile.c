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
#include "gw_tclsci.h"
#include "TCL_Command.h"
#include "TCL_Global.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "GlobalTclInterp.h"

#include "FileExist.h"
#include "TCL_getErrorLine.h"
#include "getshortpathname.h"

#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_EvalFile(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    char* l1 = NULL;
    int* piAddrl2 = NULL;
    char* l2 = NULL;

    int m1, n1;
    int m2, n2;
    int RET;

    Tcl_Interp *TCLinterpreter = NULL;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        if (getAllocatedSingleString(pvApiCtx, piAddrl1, &l1))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
            return 1;
        }

        // Check if there is a global interpreter
        TCLinterpreter = getTclInterp();
        releaseTclInterp();
        if (TCLinterpreter == NULL)
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            freeAllocatedSingleString(l1);
            return 0;
        }

        // Check if the file to load exists
        if (!FileExist((l1)))
        {
            Scierror(999, _("%s: File %s not found.\n"), fname, (l1));
            freeAllocatedSingleString(l1);
            return 0;
        }

        if (nbInputArgument(pvApiCtx) == 2)
        {
            // two arguments given - get a pointer on the slave interpreter
            if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
            {
                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
                if (sciErr.iErr)
                {
                    freeAllocatedSingleString(l1);
                    printError(&sciErr, 0);
                    return 1;
                }

                // Retrieve a matrix of double at position 2.
                if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
                {
                    freeAllocatedSingleString(l1);
                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
                    return 1;
                }

                TCLinterpreter = Tcl_GetSlave(getTclInterp(), (l2));
                releaseTclInterp();
                if (TCLinterpreter == NULL)
                {
                    freeAllocatedSingleString(l1);
                    Scierror(999, _("%s: No such slave interpreter.\n"), fname);
                    return 0;
                }

                {
                    BOOL bOK = FALSE;
                    char *sz = (l1);
                    char *szShort = getshortpathname(sz, &bOK);
                    RET = sendTclFileToSlave(szShort, (l2));
                    FREE(szShort);
                }

                freeAllocatedSingleString(l2);
            }
            else
            {
                freeAllocatedSingleString(l1);
                Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            BOOL bOK = FALSE;
            char *sz = (l1);
            char *szShort = getshortpathname(sz, &bOK);
            RET = sendTclFile(szShort);
            FREE(szShort);
        }

        if (RET == TCL_ERROR)
        {
            const char *trace = Tcl_GetVar(TCLinterpreter, "errorInfo", TCL_GLOBAL_ONLY);
            Scierror(999, _("%s, at line %i of file %s\n	%s.\n"), fname, TCL_getErrorLine(TCLinterpreter), (l1), (char *)trace);
            freeAllocatedSingleString(l1);
            return 0;
        }

        freeAllocatedSingleString(l1);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
