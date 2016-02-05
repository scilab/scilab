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
/* file: set_grid_thickness_property.c                                    */
/* desc : function to modify in Scilab the grid thickness field of        */
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
int set_grid_thickness_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status[3];
    int i = 0;
    double dGridThickness = 0;
    double* pdGridThickness = &dGridThickness;
    double gridStyles[3];
    int const gridThicknessPropertiesNames[3] = {__GO_X_AXIS_GRID_THICKNESS__, __GO_Y_AXIS_GRID_THICKNESS__, __GO_Z_AXIS_GRID_THICKNESS__};

    double* values = (double*)_pvData;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "grid_thickness");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow != 1 || nbCol > 3)
    {
        Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "grid_thickness", "1x2, 1x3");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, gridThicknessPropertiesNames[0], jni_double, (void**)&pdGridThickness);

    if (pdGridThickness == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_thickness");
        return SET_PROPERTY_ERROR;
    }

    gridStyles[0] = dGridThickness;

    getGraphicObjectProperty(iObjUID, gridThicknessPropertiesNames[1], jni_double, (void**)&pdGridThickness);
    gridStyles[1] = dGridThickness;

    getGraphicObjectProperty(iObjUID, gridThicknessPropertiesNames[2], jni_double, (void**)&pdGridThickness);
    gridStyles[2] = dGridThickness;

    for ( i = 0 ; i < nbCol ; i++)
    {
        gridStyles[i] = values[i];
    }

    status[0] = setGraphicObjectProperty(iObjUID, gridThicknessPropertiesNames[0], &gridStyles[0], jni_double, 1);
    status[1] = setGraphicObjectProperty(iObjUID, gridThicknessPropertiesNames[1], &gridStyles[1], jni_double, 1);
    status[2] = setGraphicObjectProperty(iObjUID, gridThicknessPropertiesNames[2], &gridStyles[2], jni_double, 1);

    if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid_thickness");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
