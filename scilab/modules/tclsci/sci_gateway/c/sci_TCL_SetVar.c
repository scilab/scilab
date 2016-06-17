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
#include "setvar.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "GlobalTclInterp.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_SetVar(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl2 = NULL;
    char* l2 = NULL;

    int* piAddrl1 = NULL;
    int* piAddrStr = NULL;
    char *VarName = NULL;

    static int n1, m1;
    static int n2, m2;

    int paramoutINT = 0;
    Tcl_Interp *TCLinterpreter = NULL;

    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (getTclInterp() == NULL)
    {
        releaseTclInterp();
        Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
        return 0;
    }
    releaseTclInterp();

    if (nbInputArgument(pvApiCtx) == 3)
    {
        // three arguments given - get a pointer on the slave interpreter
        if (checkInputArgumentType(pvApiCtx, 3, sci_strings))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 3.
            if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 3);
                return 1;
            }

            TCLinterpreter = Tcl_GetSlave(getTclInterp(), (l2));
            freeAllocatedSingleString(l2);
            if (TCLinterpreter == NULL)
            {
                releaseTclInterp();
                Scierror(999, _("%s: No such slave interpreter.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 3);
            return 0;
        }
    }
    else
    {
        // only two arguments given - use the main interpreter
        TCLinterpreter = getTclInterp();
    }

    if (checkInputArgumentType(pvApiCtx, 1, sci_strings) && checkInputArgumentType(pvApiCtx, 2, sci_strings))
    {
        char **Str = NULL;

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

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrStr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(VarName);
            return 1;
        }

        // Retrieve a matrix of string at position 2.
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrStr, &m1, &n1, &Str))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 2);
            freeAllocatedSingleString(VarName);
            return 1;
        }


        // Efface valeur precedente
        Tcl_UnsetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);

        if ( (m1 == 1) && (n1 == 1) )
        {
            paramoutINT = SetVarAString(TCLinterpreter, VarName, Str);
        }
        else
        {
            paramoutINT = SetVarStrings(TCLinterpreter, VarName, Str, m1, n1);
        }

        freeAllocatedSingleString(VarName);
        freeAllocatedMatrixOfString(m1, n1, Str);
    }
    else if (checkInputArgumentType(pvApiCtx, 1, sci_strings) && checkInputArgumentType(pvApiCtx, 2, sci_matrix))
    {
#define COMPLEX 1
        int *header = NULL;
        int Cmplx;
        double* l1 = NULL;

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

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedSingleString(VarName);
            return 1;
        }

        if (isVarComplex(pvApiCtx, piAddrl1))
        {
            Scierror(999, _("This function doesn't work with Complex.\n"));
            freeAllocatedSingleString(VarName);
            releaseTclInterp();
            return 0;
        }

        // Retrieve a matrix of double at position 2.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 2);
            freeAllocatedSingleString(VarName);
            return 1;
        }

        if ( (m1 == 0) && (n1 == 0) )
        {
            Scierror(999, _("[] doesn't work with Tcl/Tk.\n"));
            freeAllocatedSingleString(VarName);
            releaseTclInterp();
            return 0;
        }

        if ( (m1 == 1) && (n1 == 1) )
        {
            paramoutINT = SetVarScalar(TCLinterpreter, VarName, *l1);
        }
        else
        {
            paramoutINT = SetVarMatrix(TCLinterpreter, VarName, l1, m1, n1);
        }

        freeAllocatedSingleString(VarName);
    }
    else
    {
        if ((!checkInputArgumentType(pvApiCtx, 1, sci_strings)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname , 1);
        }
        if ((!checkInputArgumentType(pvApiCtx, 2, sci_matrix)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), fname , 2);
        }
        releaseTclInterp();
        return 0;
    }

    if (createScalarBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, paramoutINT))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    releaseTclInterp();

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
