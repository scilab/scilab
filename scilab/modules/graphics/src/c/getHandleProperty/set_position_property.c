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
int set_position_property(char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    char* type;
    BOOL status;

    /*
     * Deactivated for now.
     * Implemented for SCI_FIGURE as POSITION and SIZE set calls
     */
#if 0
    if (sciGetEntityType(pobj) == SCI_UICONTROL || sciGetEntityType(pobj) == SCI_FIGURE)
    {
        SetUicontrolPosition(pobj, stackPointer, valueType, nbRow, nbCol);
        return SET_PROPERTY_SUCCEED;
    }
#endif

    /* Deactivated for now */
#if 0
    if ( sciGetEntityType(pobj) == SCI_UIMENU )
    {
        pUIMENU_FEATURE(pobj)->MenuPosition = (int) getDoubleFromStack( stackPointer );
        return SET_PROPERTY_SUCCEED;
    }
#endif

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_string, &type);

    if ( !isParameterDoubleMatrix( valueType ) && (strcmp(type, __GO_UICONTROL__) != 0))
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "position");
        return SET_PROPERTY_ERROR;
    }

    /* Type test required since a position set requires a 4-element, 3-element, and 2-element vector
    for respectively the Figure, Label and Legend */
    if (strcmp(type, __GO_FIGURE__) == 0)
    {
        double* values;
        int figurePosition[2];
        int figureSize[2];
        int status1, status2;

        if (nbCol != 4 || nbRow != 1)
        {
            Scierror(999, _("Wrong size for '%s' property: A 1 x %d real row vector expected.\n"), "position", 4);
            return SET_PROPERTY_ERROR;
        }

        values = getDoubleMatrixFromStack( stackPointer );

        figurePosition[0] = (int) values[0];
        figurePosition[1] = (int) values[1];
        figureSize[0] = (int) values[2];
        figureSize[1] = (int) values[3];

        status = setGraphicObjectProperty(pobjUID, __GO_POSITION__, figurePosition, jni_int_vector, 2);

        if (status == TRUE)
        {
            status1 = SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "figure_position");
            status1 = SET_PROPERTY_ERROR;
        }

        status = setGraphicObjectProperty(pobjUID, __GO_AXES_SIZE__, figureSize, jni_int_vector, 2);

        if (status == TRUE)
        {
            status2 = SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "figure_size");
            status2 = SET_PROPERTY_ERROR;
        }

        return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 );
    }
    else if (strcmp(type, __GO_LABEL__) == 0)
    {
        double* values = getDoubleMatrixFromStack( stackPointer );
        double* currentPosition;
        double labelPosition[3];

        getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_double_vector, &currentPosition);

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
    else if (strcmp(type, __GO_LEGEND__) == 0)
    {
        double * values = getDoubleMatrixFromStack( stackPointer );

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
    else if (strcmp(type, __GO_UICONTROL__) == 0)
    {
        return SetUicontrolPosition(pobjUID, stackPointer, valueType, nbRow, nbCol);
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
