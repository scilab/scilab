
/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Fabrice Leray
* Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
* Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*------------------------------------------------------------------------*/
/* file: sci_rotate_axes.c                                                */
/* desc : set the message about the rotation in the info box.             */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "HandleManagement.h"
#include "getPropertyAssignedValue.h"
#include "getGraphicObjectProperty.h"
/*--------------------------------------------------------------------------*/
int sci_rotate_axes(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrstackPointer = NULL;
    long long* stackPointer = NULL;

    int nbRow = 0;
    int nbCol = 0;

    int iUID = 0;
    int* piUID = &iUID;
    int iType = -1;
    int *piType = &iType;

    /* check size of input and output */
    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        iUID = getCurrentFigure();
    }
    else
    {
        /* Get figure or subwin handle */
        if ((!checkInputArgumentType(pvApiCtx, 1, sci_handles)))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single Figure or Axes handle expected.\n"), fname, 1);
            return -1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrstackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of handle at position 1.
        sciErr = getMatrixOfHandle(pvApiCtx, piAddrstackPointer, &nbRow, &nbCol, &stackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
            return 1;
        }


        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single Figure or Axes handle expected.\n"), fname, 1);
            return -1;
        }

        iUID = getObjectFromHandle((long int) * stackPointer);

        getGraphicObjectProperty(iUID, __GO_TYPE__, jni_int, (void **)&piType);
        if (iType == __GO_AXES__)
        {
            iUID = getParentObject(iUID);
        }
    }

    if (iUID == 0)
    {
        Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
        return -1;
    }

    setGraphicObjectProperty(iUID, __GO_INFO_MESSAGE__, "Right click and drag to rotate.", jni_string, 1);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
