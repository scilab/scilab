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
#include "SetUicontrolPosition.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_position_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    int type = -1;
    int *piType = &type;
    BOOL status = FALSE;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_UICONTROL__ || type == __GO_FIGURE__)
    {
        return SetUicontrolPosition(pobjUID, stackPointer, valueType, nbRow, nbCol);
    }

    /* Type test required since a position set requires a 3-element, and 2-element vector
    for respectively the Label and Legend */
    if (type == __GO_LABEL__)
    {
        double* values = stk( stackPointer );
        double* currentPosition;
        double labelPosition[3];

        getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_double_vector, (void **)&currentPosition);

        labelPosition[0] = values[0];
        labelPosition[1] = values[1];
        labelPosition[2] = currentPosition[2];

        status = setGraphicObjectProperty(pobjUID, __GO_POSITION__, labelPosition, jni_double_vector, 3);

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
        double * values = stk( stackPointer );

        status = setGraphicObjectProperty(pobjUID, __GO_POSITION__, values, jni_double_vector, 2);

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
