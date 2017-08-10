/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
#include "InitTclTk.h"
#include "Scierror.h"
#include "localization.h"
#include "InitializeTclTk.h"
#include "GlobalTclInterp.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_DeleteInterp(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl2 = NULL;
    char* l2 = NULL;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 1)
    {

        if (!existsGlobalInterp())
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            return 0;
        }

        if (checkInputArgumentType(pvApiCtx, 1, sci_strings))
        {
            static int n2, m2;
            Tcl_Interp *TCLinterpreter = NULL;

            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
                return 1;
            }
            TCLinterpreter = Tcl_GetSlave(getTclInterp(), (l2));
            freeAllocatedSingleString(l2);
            releaseTclInterp();
            if (TCLinterpreter == NULL)
            {
                Scierror(999, _("%s: No such slave interpreter.\n"), fname);
                return 0;
            }
            else
            {
                Tcl_DeleteInterp(TCLinterpreter);
                TCLinterpreter = NULL;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
            return 0;
        }
    }
    else // nbInputArgument(pvApiCtx) == 0
    {
        releaseTclInterp();

        CloseTCLsci();
        InitializeTclTk();
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
