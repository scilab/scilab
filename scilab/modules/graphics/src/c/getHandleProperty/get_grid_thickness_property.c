/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_grid_thickness_property.c                                    */
/* desc : function to retrieve in Scilab the grid thickness field of      */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_grid_thickness_property(void* _pvCtx, int iObjUID)
{
    double grid[3];
    double dGridThickness = 0;
    double * pdGridThickness = &dGridThickness;
    int iView = 0;
    int* piView = &iView;

    /* need conversion for display in double */
    getGraphicObjectProperty(iObjUID, __GO_X_AXIS_GRID_THICKNESS__, jni_double, (void **)&pdGridThickness);

    if (pdGridThickness == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_thickness");
        return -1;
    }

    grid[0] = dGridThickness;

    getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_GRID_THICKNESS__, jni_double, (void **)&pdGridThickness);
    if (pdGridThickness == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid");
        return -1;
    }

    grid[1] = dGridThickness;

    getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_GRID_THICKNESS__, jni_double, (void **)&pdGridThickness);
    if (pdGridThickness == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid");
        return -1;
    }

    grid[2] = dGridThickness;

    getGraphicObjectProperty(iObjUID, __GO_VIEW__, jni_int, (void **)&piView);

    if (iView)
    {
        return sciReturnRowVector(_pvCtx, grid, 3);
    }
    else
    {
        return sciReturnRowVector(_pvCtx, grid, 2);
    }
}
/*------------------------------------------------------------------------*/
