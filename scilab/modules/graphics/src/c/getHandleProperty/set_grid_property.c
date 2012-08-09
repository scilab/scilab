/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_grid_property.c                                              */
/* desc : function to modify in Scilab the grid field of                  */
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
int set_grid_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status[3];
    int i = 0;
    int iGridColor = 0;
    int* piGridColor = &iGridColor;
    int gridStyles[3];
    char* gridColorPropertiesNames[3] = {__GO_X_AXIS_GRID_COLOR__, __GO_Y_AXIS_GRID_COLOR__, __GO_Z_AXIS_GRID_COLOR__};

    double * values = stk( stackPointer );

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "grid");
        return SET_PROPERTY_ERROR;
    }

    if ( nbRow != 1 || nbCol > 3 )
    {
        Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "grid", "1x2, 1x3");
        return SET_PROPERTY_ERROR ;
    }

    getGraphicObjectProperty(pobjUID, gridColorPropertiesNames[0], jni_int, (void**)&piGridColor);

    if (piGridColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid");
        return SET_PROPERTY_ERROR;
    }

    gridStyles[0] = iGridColor;

    getGraphicObjectProperty(pobjUID, gridColorPropertiesNames[1], jni_int, (void**)&piGridColor);
    gridStyles[1] = iGridColor;

    getGraphicObjectProperty(pobjUID, gridColorPropertiesNames[2], jni_int, (void**)&piGridColor);
    gridStyles[2] = iGridColor;

    for (  i = 0 ; i < nbCol ; i++ )
    {
        int curValue = (int) values[i];
        if ( values[i] < -1 || !sciCheckColorIndex(pobjUID, curValue) )
        {
            Scierror(999, _("Wrong value for '%s' property: Must be -1 or a valid color index.\n"), "grid");
            return SET_PROPERTY_ERROR ;
        }
        gridStyles[i] = curValue ;
    }

    status[0] = setGraphicObjectProperty(pobjUID, gridColorPropertiesNames[0], &gridStyles[0], jni_int, 1);
    status[1] = setGraphicObjectProperty(pobjUID, gridColorPropertiesNames[1], &gridStyles[1], jni_int, 1);
    status[2] = setGraphicObjectProperty(pobjUID, gridColorPropertiesNames[2], &gridStyles[2], jni_int, 1);

    if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "grid");
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
