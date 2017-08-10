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
/* file: set_grid_style_property.c                                        */
/* desc : function to modify in Scilab the grid style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_grid_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status[3];
    int i = 0;
    int iGridStyle = 0;
    int* piGridStyle = &iGridStyle;
    int gridStyles[3];
    int const gridStylePropertiesNames[3] = {__GO_X_AXIS_GRID_STYLE__, __GO_Y_AXIS_GRID_STYLE__, __GO_Z_AXIS_GRID_STYLE__};

    double* values = (double*)_pvData;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "grid_style");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow != 1 || nbCol > 3)
    {
        Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "grid_style", "1x2, 1x3");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, gridStylePropertiesNames[0], jni_int, (void**)&piGridStyle);

    if (piGridStyle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_style");
        return SET_PROPERTY_ERROR;
    }

    gridStyles[0] = iGridStyle;

    getGraphicObjectProperty(iObjUID, gridStylePropertiesNames[1], jni_int, (void**)&piGridStyle);
    gridStyles[1] = iGridStyle;

    getGraphicObjectProperty(iObjUID, gridStylePropertiesNames[2], jni_int, (void**)&piGridStyle);
    gridStyles[2] = iGridStyle;

    for ( i = 0 ; i < nbCol ; i++)
    {
        gridStyles[i] = (int) values[i];
    }

    status[0] = setGraphicObjectProperty(iObjUID, gridStylePropertiesNames[0], &gridStyles[0], jni_int, 1);
    status[1] = setGraphicObjectProperty(iObjUID, gridStylePropertiesNames[1], &gridStyles[1], jni_int, 1);
    status[2] = setGraphicObjectProperty(iObjUID, gridStylePropertiesNames[2], &gridStyles[2], jni_int, 1);

    if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_style");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
