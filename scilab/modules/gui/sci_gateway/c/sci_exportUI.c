/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
 * desc : interface for xs2png routine
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
#include "stack-c.h"
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"
#include "exportUserInterface.hxx"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_exportUI(char * fname, unsigned long fname_len)
{
    int iFigureId = 0; // id of the figure to export
    int iRows = 0;
    int iCols = 0;
    size_t stackPointer = 0;

    CheckLhs(0, 1);
    CheckRhs(1, 1);

    if (GetType(1) == sci_handles) // exportUI(figHandle)
    {
        char *pstFigureUID = NULL;
        int iHandleType = -1;
        int *piHandleType = &iHandleType;
        int *piFigureId = &iFigureId;

        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &iRows, &iCols, &stackPointer);
        if (iRows * iCols != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
        }

        pstFigureUID = getObjectFromHandle((unsigned long) * (hstk(stackPointer)));

        getGraphicObjectProperty(pstFigureUID, __GO_TYPE__, jni_int, (void **)&piHandleType);
        if (iHandleType == __GO_FIGURE__)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
            return FALSE;
        }

        getGraphicObjectProperty(pstFigureUID, __GO_ID__, jni_int, (void **)&piFigureId);
    }
    else if (GetType(1) == sci_matrix) // exportUI(figId)
    {
        GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &stackPointer);
        if (iRows * iCols != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
            return FALSE;
        }

        iFigureId = (int) * (stk(stackPointer));
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Real Scalar or a 'Figure' handle expected.\n"), fname, 1);
        return FALSE;
    }

    // call the export function
    exportUserInterface(iFigureId);

    LhsVar(1) = 0;

    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
