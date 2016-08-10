/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"
#include "FigureList.h"
#include "HandleManagement.h"
#include "GetProperty.h"
#include "freeArrayOfString.h"
#include "os_string.h"
#include "configvariable_interface.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "getConsoleIdentifier.h"
/*--------------------------------------------------------------------------*/
int sci_toolbar(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddr1 = NULL;
    int* piStkAdr = NULL;
    int* piAddrstkAdr = NULL;
    long long* stkAdr = NULL;
    int* piAddrparam = NULL;

    int nbCol = 0;
    int nbRow = 0;

    char *Output = NULL;
    char **param = NULL;
    int figNum = -2;

    int iIsVisible = 0;
    int *piIsVisible = NULL;

    int iParentUID = 0;
    int iParentType = -1;
    int *piParentType = &iParentType;

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /* Figure number */
    if (checkInputArgumentType(pvApiCtx, 1, sci_matrix))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr1, &nbRow, &nbCol, &piStkAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
            return 1;
        }

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real expected.\n"), fname, 1);
            return FALSE;
        }
        figNum = *piStkAdr;

        if (figNum < -1)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be > %d expected.\n"), fname, 1, -1);
            return FALSE;
        }

        if (figNum != -1)       /* Check that the figure exists */
        {
            if (getFigureFromIndex(figNum) == 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: 'Graphic Window Number %d' does not exist.\n"), fname, 1, figNum);
                return FALSE;
            }
        }

        if (figNum == -1)
        {
            iParentUID = getConsoleIdentifier();
        }
        else
        {
            iParentUID = getFigureFromIndex(figNum);
        }
    }
    else if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrstkAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of handle at position 1.
        sciErr = getMatrixOfHandle(pvApiCtx, piAddrstkAdr, &nbRow, &nbCol, &stkAdr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
            return 1;
        }

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return FALSE;
        }

        iParentUID = getObjectFromHandle((long) * stkAdr);

        if (iParentUID == 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: this handle does not exist.\n"), fname, 1);
            return FALSE;
        }

        getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
        if (iParentType != __GO_FIGURE__)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real or a Figure handle expected.\n"), fname, 1);
            return FALSE;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real or a Figure handle expected.\n"), fname, 1);
        return FALSE;
    }

    if (nbInputArgument(pvApiCtx) == 2)               /* New status */
    {
        if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrparam);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of string at position 2.
            if (getAllocatedMatrixOfString(pvApiCtx, piAddrparam, &nbRow, &nbCol, &param))
            {
                Scierror(202, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                freeAllocatedMatrixOfString(nbRow, nbCol, param);
                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 2);
                return FALSE;
            }

            if ((strcmp(param[0], "off") == 0) || (strcmp(param[0], "on") == 0))
            {
                iIsVisible = strcmp(param[0], "on") == 0;
                if (iParentUID != getConsoleIdentifier() || getScilabMode() == SCILAB_STD)
                {
                    setGraphicObjectProperty(iParentUID, __GO_TOOLBAR_VISIBLE__, &iIsVisible, jni_bool, 1);
                }
                freeAllocatedMatrixOfString(nbRow, nbCol, param);
            }
            else
            {
                freeAllocatedMatrixOfString(nbRow, nbCol, param);
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 2, "on", "off");
                return FALSE;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return FALSE;
        }
    }

    /* Returned value */
    piIsVisible = &iIsVisible;
    getGraphicObjectProperty(iParentUID, __GO_TOOLBAR_VISIBLE__, jni_bool, (void **)&piIsVisible);
    if (iIsVisible)
    {
        Output = os_strdup("on");
    }
    else
    {
        Output = os_strdup("off");
    }

    nbCol = 1;
    nbRow = (int)strlen(Output);
    if (createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, Output))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    FREE(Output);
    Output = NULL;

    ReturnArguments(pvApiCtx);
    return TRUE;
}

/*--------------------------------------------------------------------------*/
