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
#include <stdio.h>
#include <string.h>
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
#include "TCL_ArrayExist.h"
#include "TCL_ArrayDim.h"
#include "TCL_ArrayGetVar.h"
#include "sci_malloc.h"
#include "GlobalTclInterp.h"
#include "os_string.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_GetVar(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    int* piAddrl2 = NULL;
    char *VarName = NULL;
    char* l2 = NULL;

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

                TCLinterpreter = Tcl_GetSlave(getTclInterp(), l2);
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

        if (TCL_ArrayExist(TCLinterpreter, VarName))
        {
            int j = 0;
            int nb_lines = 0, nb_columns = 0;
            char **index_list = TCL_ArrayDim(TCLinterpreter, VarName, &nb_lines, &nb_columns);

            if (index_list)
            {
                char **ReturnArrayString = MALLOC(nb_lines * nb_columns * sizeof(char*));
                if (ReturnArrayString)
                {
                    for (j = 0 ; j < nb_lines * nb_columns ; j++)
                    {
                        ReturnArrayString[j] = TCL_ArrayGetVar(TCLinterpreter, VarName, index_list[j]);
                    }

                    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, nb_lines, nb_columns, ReturnArrayString);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        freeArrayOfString(ReturnArrayString, nb_lines * nb_columns);
                        freeArrayOfString(index_list, nb_lines * nb_columns);
                        freeAllocatedSingleString(VarName);
                        return 1;
                    }

                    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

                    freeArrayOfString(ReturnArrayString, nb_lines * nb_columns);
                    freeArrayOfString(index_list, nb_lines * nb_columns);
                    ReturnArguments(pvApiCtx);
                }
                else
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    freeArrayOfString(index_list, nb_lines * nb_columns);
                    freeAllocatedSingleString(VarName);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                freeAllocatedSingleString(VarName);
                return 0;
            }
        }
        else
        {
            char *RetStr = (char*)Tcl_GetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);
            if ( RetStr )
            {
                char *output = os_strdup(RetStr);
                sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &output);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    freeAllocatedSingleString(VarName);
                    return 1;
                }

                if (output)
                {
                    FREE(output);
                    output = NULL;
                }
                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: Could not read Tcl Variable.\n"), "TCL_GetVar");
                freeAllocatedSingleString(VarName);
                releaseTclInterp();
                return 0;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        releaseTclInterp();
        return 0;
    }

    freeAllocatedSingleString(VarName);
    releaseTclInterp();
    return 0;
}
/*--------------------------------------------------------------------------*/
