/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: sci_show_window.c                                                */
/* desc : interface for show_window routine                               */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "gw_graphics.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "localization.h"
#include "Interaction.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "createGraphicObject.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BuildObjects.h"
#include "CurrentSubwin.h"
#include "sci_types.h"
/*--------------------------------------------------------------------------*/
int sci_show_window(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrstackPointer = NULL;
    long long* llstackPointer = NULL;
    double* pdblstackPointer = NULL;

    int iFigureUID = 0;
    int iAxesUID = 0;
    int* piAxesUID = &iAxesUID;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 1)
    {
        /* the window to show is specified */
        int paramType    = getInputArgumentType(pvApiCtx, 1);
        int nbRow        = 0;
        int nbCol        = 0;

        int type = -1;
        int *piType = &type;

        sciErr = getVarAddressFromPosition(pvApiCtx,  1, &piAddrstackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if ((paramType == sci_handles))
        {
            /* by tis handle */
            // Retrieve a matrix of handle at position  1.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrstackPointer, &nbRow, &nbCol, &llstackPointer);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname,  1);
                return 1;
            }


            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle or a real scalar expected.\n"), fname, 1, "Figure");
                return -1;
            }

            iFigureUID = getObjectFromHandle((long int)(*llstackPointer));

            if (iFigureUID == 0)
            {
                Scierror(999, _("%s: Handle does not or no longer exists.\n"), fname);
                return -1;
            }

            getGraphicObjectProperty(iFigureUID, __GO_TYPE__, jni_int, (void **) &piType);
            if (type != __GO_FIGURE__)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle or a real scalar expected.\n"), fname, 1, "Figure");
                return -1;
            }

        }
        else if ((paramType == sci_matrix))
        {
            /* by its number */
            int winNum = 0;
            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrstackPointer, &nbRow, &nbCol, &pdblstackPointer);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle or a real scalar expected.\n"), fname, 1, "Figure");
                return -1;
            }
            winNum = (int) * pdblstackPointer;
            iFigureUID = getFigureFromIndex(winNum);

            if (iFigureUID == 0)
            {
                iFigureUID = createNewFigureWithAxes();
                setGraphicObjectProperty(iFigureUID, __GO_ID__, &winNum, jni_int, 1);
                setCurrentFigure(iFigureUID);

                getGraphicObjectProperty(iFigureUID, __GO_SELECTED_CHILD__, jni_int,  (void**)&piAxesUID);
                setCurrentSubWin(iAxesUID);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle or a real scalar expected.\n"), fname, 1, "Figure");
            return -1;
        }
    }
    else
    {
        /* nbInputArgument(pvApiCtx) == 0 */
        /* raise current figure */
        getOrCreateDefaultSubwin();
        iFigureUID = getCurrentFigure();
    }

    /* Check that the requested figure really exists */
    if (iFigureUID == 0)
    {
        Scierror(999, _("%s: '%s' handle does not or no longer exists.\n"), fname, "Figure");
        return -1;
    }

    /* Actually show the window */
    showWindow(iFigureUID);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
