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
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
int sci_rotate_axes(char *fname,unsigned long fname_len)
{
    int nbRow = 0;
    int nbCol = 0;
    size_t stackPointer = 0;

    char* pstrUID = NULL;
    int iType = -1;
    int *piType = &iType;

    /* check size of input and output */
    CheckRhs(0,1);
    CheckLhs(0,1);

    if(Rhs == 0)
    {
        pstrUID = (char*)getCurrentFigure();
    }
    else
    {
        /* Get figure or subwin handle */
        if (GetType(1) != sci_handles)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single Figure or Axes handle expected.\n"), fname, 1);
            return -1;
        }

        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer);

        if (nbRow * nbCol != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single Figure or Axes handle expected.\n"), fname, 1);
            return -1;
        }

        pstrUID = (char*)getObjectFromHandle(getHandleFromStack(stackPointer));

        getGraphicObjectProperty(pstrUID, __GO_TYPE__, jni_int, (void **)&piType);
        if (iType == __GO_AXES__)
        {
            getGraphicObjectProperty(pstrUID, __GO_PARENT__, jni_string, (void **)&pstrUID);
        }
    }

    if (pstrUID == NULL)
    {
        Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
        return -1;
    }

    setGraphicObjectProperty(pstrUID, __GO_INFO_MESSAGE__, "Right click and drag to rotate.", jni_string, 1);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
