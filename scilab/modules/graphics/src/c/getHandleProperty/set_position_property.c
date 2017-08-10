/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2013 - Pedro SOUZA
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
/* file: set_position_property.c                                          */
/* desc : function to modify in Scilab the position field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetUicontrol.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_position_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int type = -1;
    int *piType = &type;
    BOOL status = FALSE;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_UICONTROL__ || type == __GO_FIGURE__)
    {
        return SetUicontrolPosition(iObjUID, _pvData, valueType, nbRow, nbCol);
    }

    /* Type test required since a position set requires a 3-element, and 2-element vector
    for respectively the Label and Legend */
    if (type == __GO_LABEL__)
    {
        double* values = (double*)_pvData;
        double* currentPosition;
        double labelPosition[3];

        getGraphicObjectProperty(iObjUID, __GO_POSITION__, jni_double_vector, (void **)&currentPosition);

        labelPosition[0] = values[0];
        labelPosition[1] = values[1];
        labelPosition[2] = currentPosition[2];

        status = setGraphicObjectProperty(iObjUID, __GO_POSITION__, labelPosition, jni_double_vector, 3);

        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return SET_PROPERTY_ERROR;
        }

    }
    else if (type == __GO_LEGEND__)
    {
        double* values = (double*)_pvData;

        status = setGraphicObjectProperty(iObjUID, __GO_POSITION__, values, jni_double_vector, 2);

        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return SET_PROPERTY_ERROR;
        }

    }
    else if (type == __GO_LIGHT__)
    {
        double* values = (double*)_pvData;

        if (valueType != sci_matrix)
        {
            Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "position");
            return SET_PROPERTY_ERROR;
        }

        if (nbRow * nbCol != 3)
        {
            Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "position", 3);
            return SET_PROPERTY_ERROR;
        }

        status = setGraphicObjectProperty(iObjUID, __GO_POSITION__, values, jni_double_vector, 3);

        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
