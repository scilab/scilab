/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

#include "gw_gui.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "InitUIMenu.h"
#include "configvariable_interface.h"
#include "FigureList.h"
#include "getConsoleIdentifier.h"
/*--------------------------------------------------------------------------*/
int sci_unsetmenu(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrmenuNameAdr      = NULL;
    char* menuNameAdr           = NULL;
    int* piAddrfigureNumberAdr  = NULL;
    double* figureNumberAdr     = NULL;
    int* piAddrsubMenuIndexAdr  = NULL;
    double* subMenuIndexAdr     = NULL;

    int nbRow = 0;
    int nbCol = 0;

    // Check parameter number
    CheckInputArgument(pvApiCtx, 1, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 1)
    {
        // Error message in not in standard mode (we need figure number)
        if (getScilabMode() != SCILAB_STD)
        {
            Scierror(999, _("%s: Figure number must be given when not in '%s' mode.\n"), fname, "STD");
            return FALSE;
        }

        // Unset a Menu of Scilab Main Window
        if ((!checkInputArgumentType(pvApiCtx, 1, sci_strings)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return FALSE;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrmenuNameAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        if (getAllocatedSingleString(pvApiCtx, piAddrmenuNameAdr, &menuNameAdr))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
            return 1;
        }

        EnableMenu(getConsoleIdentifier(), menuNameAdr, FALSE);
        freeAllocatedSingleString(menuNameAdr);
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        // Unset a Menu a Scilab Graphic Window
        if ((checkInputArgumentType(pvApiCtx, 1, sci_matrix)) && (checkInputArgumentType(pvApiCtx, 2, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrfigureNumberAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrfigureNumberAdr, &nbRow, &nbCol, &figureNumberAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }


            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return FALSE;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrmenuNameAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            if (getAllocatedSingleString(pvApiCtx, piAddrmenuNameAdr, &menuNameAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                return 1;
            }

            EnableMenu(getFigureFromIndex((int)*figureNumberAdr), menuNameAdr, FALSE);
            freeAllocatedSingleString(menuNameAdr);
        }
        else if ((checkInputArgumentType(pvApiCtx, 1, sci_strings)) && (checkInputArgumentType(pvApiCtx, 2, sci_matrix))) // Unset a submenu in main window
        {
            // Error message in not in standard mode (we need figure number)
            if (getScilabMode() != SCILAB_STD)
            {
                Scierror(999, _("%s: Figure number must be given when not in '%s' mode.\n"), fname, "STD");
                return FALSE;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrmenuNameAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddrmenuNameAdr, &menuNameAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
                return 1;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrsubMenuIndexAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(menuNameAdr);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrsubMenuIndexAdr, &nbRow, &nbCol, &subMenuIndexAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
                freeAllocatedSingleString(menuNameAdr);
                return 1;
            }


            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 2);
                freeAllocatedSingleString(menuNameAdr);
                return FALSE;
            }

            EnableSubMenu(getConsoleIdentifier(), menuNameAdr, (int)*subMenuIndexAdr, FALSE);
            freeAllocatedSingleString(menuNameAdr);
        }
        else
        {
            Scierror(999, _("%s: Wrong input arguments: '%s' or '%s' expected.\n"), fname, "(button, nsub)", "(gwin, button)");
            return FALSE;
        }
    }
    else                        // Unset a submenu in graphics window
    {
        if ((checkInputArgumentType(pvApiCtx, 1, sci_matrix)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrfigureNumberAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrfigureNumberAdr, &nbRow, &nbCol, &figureNumberAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }


            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
            return FALSE;
        }

        if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrmenuNameAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            if (getAllocatedSingleString(pvApiCtx, piAddrmenuNameAdr, &menuNameAdr))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return FALSE;
        }

        if ((checkInputArgumentType(pvApiCtx, 3, sci_matrix)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrsubMenuIndexAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(menuNameAdr);
                return 1;
            }

            // Retrieve a matrix of double at position 3.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrsubMenuIndexAdr, &nbRow, &nbCol, &subMenuIndexAdr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
                freeAllocatedSingleString(menuNameAdr);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 3);
                freeAllocatedSingleString(menuNameAdr);
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 3);
            freeAllocatedSingleString(menuNameAdr);
            return FALSE;
        }

        EnableSubMenu(getFigureFromIndex((int)*figureNumberAdr), menuNameAdr, (int)*subMenuIndexAdr, FALSE);
        freeAllocatedSingleString(menuNameAdr);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
