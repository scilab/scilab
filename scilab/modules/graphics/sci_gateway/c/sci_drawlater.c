/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
/* file: sci_drawlater.c                                                  */
/* desc : interface for drawlater routine                                 */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "BuildObjects.h"
#include "CurrentObject.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
/*--------------------------------------------------------------------------*/
int sci_drawlater(char * fname, void* pvApiCtx)
{
    int iFalse =  (int)FALSE;
    int iParentFigureUID = 0;
    int* piParentFigureUID = &iParentFigureUID;
    int iSubwinUID = 0;
    int iCurChildUID = 0;
    int iType = -1;
    int *piType = &iType;

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        iSubwinUID = getOrCreateDefaultSubwin();
        if (iSubwinUID != 0)
        {
            // Look for top level figure
            iCurChildUID = iSubwinUID;
            do
            {
                iParentFigureUID = getParentObject(iCurChildUID);
                getGraphicObjectProperty(iParentFigureUID, __GO_TYPE__, jni_int, (void **)&piType);
                iCurChildUID = iParentFigureUID;
            }
            while (iParentFigureUID != 0 && iType != __GO_FIGURE__);

            if (iParentFigureUID != 0)
            {
                setGraphicObjectProperty(iParentFigureUID, __GO_IMMEDIATE_DRAWING__, &iFalse, jni_bool, 1);
            }
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
