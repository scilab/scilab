/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 * desc : interface for xs2png routine
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
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"
#include "exportUserInterface.hxx"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "FigureList.h"
/*--------------------------------------------------------------------------*/
int sci_exportUI(char * fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrstackPointer = NULL;

    int iFigureId   = 0; // id of the figure to export
    int *piFigureId = &iFigureId;
    int iRows       = 0;
    int iCols       = 0;
    int iHandleType = -1;
    int *piHandleType = &iHandleType;

    CheckOutputArgument(pvApiCtx, 0, 1);
    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrstackPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (checkInputArgumentType(pvApiCtx, 1, sci_handles)) // exportUI(figHandle)
    {
        int iFigureUID = 0;
        long long* stackPointer = NULL;
        // Retrieve a matrix of handle at position 1.
        sciErr = getMatrixOfHandle(pvApiCtx, piAddrstackPointer, &iRows, &iCols, &stackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
            return 1;
        }

        if (iRows * iCols != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
            return 0;
        }

        iFigureUID = getObjectFromHandle((unsigned long) * stackPointer);

        getGraphicObjectProperty(iFigureUID, __GO_TYPE__, jni_int, (void **)&piHandleType);
        if (iHandleType != __GO_FIGURE__)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
            return 0;
        }

        getGraphicObjectProperty(iFigureUID, __GO_ID__, jni_int, (void **)&piFigureId);
    }
    else if (checkInputArgumentType(pvApiCtx, 1, sci_matrix)) // exportUI(figId)
    {
        double* stackPointer = NULL;

        // Retrieve a matrix of double at position 1.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrstackPointer, &iRows, &iCols, &stackPointer);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
            return 1;
        }

        if (iRows * iCols != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
            return 0;
        }

        iFigureId = (int) * stackPointer;
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
        return 0;
    }

    if (getFigureFromIndex(iFigureId) == 0)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A valid figure identifier expected.\n"), fname, 1);
        return 0;
    }

    // call the export function
    exportUserInterface(iFigureId);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
