/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
/* file: sci_newaxes.c                                                    */
/* desc : interface for newaxes routine                                   */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "gw_graphics.h"
#include "api_scilab.h"
#include "BuildObjects.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "localization.h"
#include "HandleManagement.h"

#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"
#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_newaxes(char * fname, void *pvApiCtx)
{
    SciErr sciErr;

    long long* outindex = NULL;

    int iSubwinUID = 0;
    long long hParent = 0;
    int iParentUID = -1;
    int iParentType = -1;
    int *piParentType = &iParentType;
    int iParentStyle = -1;
    int *piParentStyle = &iParentStyle;

    int iRhs = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (iRhs == 0)
    {
        getOrCreateDefaultSubwin();

        if ((iSubwinUID = createSubWin (getCurrentFigure())) != 0)
        {
            if (createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iSubwinUID)))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 1) = 1;
            ReturnArguments(pvApiCtx);
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
        }
        return 0;
    }
    else
    {
        // iRhs == 1
        int* piAddr = NULL;
        int iType = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isHandleType(pvApiCtx, piAddr) == FALSE || isScalar(pvApiCtx, piAddr) == FALSE)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarHandle(pvApiCtx, piAddr, &hParent))
        {
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
            return 1;
        }

        iParentUID = getObjectFromHandle((long)hParent);
        if (iParentUID != 0)
        {
            getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
            getGraphicObjectProperty(iParentUID, __GO_STYLE__, jni_int, (void **)&piParentStyle);
            if (!(iParentType == __GO_FIGURE__ || iParentStyle == __GO_UI_FRAME__))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or '%s' handle expected.\n"), fname, 1, "Figure",
                         "Frame");
                return FALSE;
            }

            if ((iSubwinUID = createSubWin(iParentUID)) != 0)
            {
                if (createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iSubwinUID)))
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 1;
                }

                AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
                ReturnArguments(pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
            }
            return 0;
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or '%s' handle expected.\n"), fname, 1, "Figure",
                     "Frame");
            return FALSE;
        }
    }
}
/*--------------------------------------------------------------------------*/
