/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2005-2008 - INRIA - Allan CORNET
*  Copyright (C) 2005-2008 - INRIA - Bruno JOFRET
*  Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "Scierror.h"
#include "localization.h"
#include "GlobalTclInterp.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_UpVar (char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    int* piAddrl2 = NULL;
    int* piAddrl3 = NULL;
    char* l3 = NULL;

    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings) && (checkInputArgumentType(pvApiCtx, 2, sci_strings)))
    {
        int m1 = 0, n1 = 0;
        int m2 = 0, n2 = 0;

        Tcl_Interp *TCLinterpreter = NULL;
        char *sourceName = NULL, *destName = NULL;
        int paramoutINT = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        if (getAllocatedSingleString(pvApiCtx, piAddrl1, &sourceName))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(sourceName);
            return 1;
        }

        // Retrieve a matrix of double at position 2.
        if (getAllocatedSingleString(pvApiCtx, piAddrl2, &destName))
        {
            freeAllocatedSingleString(sourceName);
            Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
            return 1;
        }

        if (getTclInterp() == NULL)
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            freeAllocatedSingleString(destName);
            freeAllocatedSingleString(sourceName);
            releaseTclInterp();
            return 0;
        }
        releaseTclInterp();

        if (nbInputArgument(pvApiCtx) == 3)
        {
            int m3 = 0, n3 = 0;
            // three arguments given - get a pointer on the slave interpreter
            if (checkInputArgumentType(pvApiCtx, 3, sci_strings))
            {
                sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl3);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    freeAllocatedSingleString(sourceName);
                    freeAllocatedSingleString(destName);
                    return 1;
                }

                // Retrieve a matrix of double at position 3.
                if (getAllocatedSingleString(pvApiCtx, piAddrl3, &l3))
                {
                    Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 3);
                    freeAllocatedSingleString(destName);
                    freeAllocatedSingleString(sourceName);
                    return 1;
                }

                TCLinterpreter = Tcl_GetSlave(getTclInterp() , (l3));
                freeAllocatedSingleString(l3);
                releaseTclInterp();
                if (TCLinterpreter == NULL)
                {
                    Scierror(999, _("%s: No such slave interpreter.\n"), fname);
                    freeAllocatedSingleString(destName);
                    freeAllocatedSingleString(sourceName);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 3);
                freeAllocatedSingleString(destName);
                freeAllocatedSingleString(sourceName);
                return 0;
            }
        }
        else
        {
            // only two arguments given - use the main interpreter
            TCLinterpreter = getTclInterp();
            releaseTclInterp();
        }

        if ( Tcl_GetVar(TCLinterpreter, sourceName, TCL_GLOBAL_ONLY) )
        {
            if ( Tcl_UpVar(TCLinterpreter, "#0", sourceName, destName, TCL_GLOBAL_ONLY) == TCL_ERROR )
            {
                paramoutINT = (int)(FALSE);
            }
            else
            {
                paramoutINT = (int)(TRUE);
            }
        }
        else
        {
            paramoutINT = (int)(FALSE);
        }

        freeAllocatedSingleString(destName);
        freeAllocatedSingleString(sourceName);

        if (createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, paramoutINT))
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d or #%d: String expected.\n"), fname, 1, 2);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
