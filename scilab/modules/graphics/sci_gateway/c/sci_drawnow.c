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
/* file: sci_drawnow.c                                                    */
/* desc : interface for drawnow routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "BuildObjects.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
/*--------------------------------------------------------------------------*/
int sci_drawnow(char *fname, unsigned long fname_len)
{
    int iTrue = (int)TRUE;
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
                setGraphicObjectProperty(iParentFigureUID, __GO_IMMEDIATE_DRAWING__, &iTrue, jni_bool, 1);
            }
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
