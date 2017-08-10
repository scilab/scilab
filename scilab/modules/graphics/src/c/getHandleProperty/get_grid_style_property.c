/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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
/* file: get_grid_style_property.c                                        */
/* desc : function to retrieve in Scilab the grid style field of          */
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
void* get_grid_style_property(void* _pvCtx, int iObjUID)
{
    double grid[3];
    int iGridStyle = 0;
    int* piGridStyle = &iGridStyle;
    int iView = 0;
    int* piView = &iView;

    /* need conversion for display in double */
    getGraphicObjectProperty(iObjUID, __GO_X_AXIS_GRID_STYLE__, jni_int, (void **)&piGridStyle);

    if (piGridStyle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_style");
        return NULL;
    }

    grid[0] = (double) iGridStyle;

    getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_GRID_STYLE__, jni_int, (void **)&piGridStyle);
    if (piGridStyle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_style");
        return NULL;
    }

    grid[1] = (double) iGridStyle;

    getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_GRID_STYLE__, jni_int, (void **)&piGridStyle);
    if (piGridStyle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_style");
        return NULL;
    }

    grid[2] = (double) iGridStyle;

    getGraphicObjectProperty(iObjUID, __GO_VIEW__, jni_int, (void **)&piView);

    if (iView)
    {
        return sciReturnRowVector(grid, 3);
    }
    else
    {
        return sciReturnRowVector(grid, 2);
    }

}
/*------------------------------------------------------------------------*/
