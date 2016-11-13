/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "TCL_ArrayExist.h"
#include "Scierror.h"
#include "localization.h"
#include "GlobalTclInterp.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_ExistArray(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    int* piAddrl2 = NULL;
    char* l2 = NULL;

    static int n1, m1;
    static int n2, m2;

    int ValRet = 0;

    Tcl_Interp *TCLinterpreter = NULL;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
    {
        char *VarName = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        if (getAllocatedSingleString(pvApiCtx, piAddrl1, &VarName))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
            return 1;
        }

        if (!existsGlobalInterp())
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            freeAllocatedSingleString(VarName);
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
                    printError(&sciErr, 0);
                    freeAllocatedSingleString(VarName);
                    return 1;
                }

                // Retrieve a matrix of double at position 2.
                if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
                    freeAllocatedSingleString(VarName);
                    return 1;
                }

                TCLinterpreter = Tcl_GetSlave(getTclInterp(), (l2));
                freeAllocatedSingleString(l2);
                if (TCLinterpreter == NULL)
                {
                    Scierror(999, _("%s: No such slave interpreter.\n"), fname);
                    freeAllocatedSingleString(VarName);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
                freeAllocatedSingleString(VarName);
                return 0;
            }
        }
        else
        {
            // only one argument given - use the main interpreter
            TCLinterpreter = getTclInterp();
        }

        ValRet = TCL_ArrayExist(TCLinterpreter, VarName);
        freeAllocatedSingleString(VarName);
        releaseTclInterp();

        if (createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, ValRet))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
