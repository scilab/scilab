/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BuildObjects.h"
#include "CurrentSubwin.h"
#include "sci_types.h"
#include "UIWidget.h"
/*--------------------------------------------------------------------------*/
int sci_show_window(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrstackPointer = NULL;
    long long* llstackPointer = NULL;
    double* pdblstackPointer = NULL;

    char* pFigureUID = NULL;
    char* pstrAxesUID = NULL;

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

            if (*llstackPointer < 0)
            {
                // UIWidget
                showWindowUIWidget(*llstackPointer);
                AssignOutputVariable(pvApiCtx, 1) = 0;
                ReturnArguments(pvApiCtx);

                return 0;
            }

            pFigureUID = (char*)getObjectFromHandle((long int)(*llstackPointer));

            if (pFigureUID == NULL)
            {
                Scierror(999, _("%s: Handle does not or no longer exists.\n"), fname);
                return -1;
            }

            getGraphicObjectProperty(pFigureUID, __GO_TYPE__, jni_int, (void **) &piType);
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
                Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
                return 1;
            }

            if (nbRow * nbCol != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A '%s' handle or a real scalar expected.\n"), fname, 1, "Figure");
                return -1;
            }
            winNum = (int) * pdblstackPointer;
            pFigureUID = (char*)getFigureFromIndex(winNum);

            if (pFigureUID == NULL)
            {
                pFigureUID = createNewFigureWithAxes();
                setGraphicObjectProperty(pFigureUID, __GO_ID__, &winNum, jni_int, 1);
                setCurrentFigure(pFigureUID);

                getGraphicObjectProperty(pFigureUID, __GO_SELECTED_CHILD__, jni_string,  (void**)&pstrAxesUID);
                setCurrentSubWin(pstrAxesUID);
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
        pFigureUID = (char*)getCurrentFigure();
    }

    /* Check that the requested figure really exists */
    if (pFigureUID == NULL)
    {
        Scierror(999, _("%s: '%s' handle does not or no longer exists.\n"), fname, "Figure");
        return -1;
    }

    /* Actually show the window */
    showWindow(pFigureUID);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
