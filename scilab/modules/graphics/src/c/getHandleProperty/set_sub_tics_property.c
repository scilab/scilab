/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
/* file: set_sub_tics_property.c                                          */
/* desc : function to modify in Scilab the sub_tics field of              */
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

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_sub_tics_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int result = 0;
    int type = -1;
    int *piType = &type;
    int axisSubticksPropertiesNames[3] = {__GO_X_AXIS_SUBTICKS__, __GO_Y_AXIS_SUBTICKS__, __GO_Z_AXIS_SUBTICKS__};

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "sub_tics");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    /*
     * Type test required as the Axis object stores subticks as a single int
     * whereas Axes maintain a 3-element int vector.
     */
    if (type == __GO_AXIS__)
    {
        int nbTicks = (int)((double*)_pvData)[0];

        status =  setGraphicObjectProperty(iObjUID, __GO_SUBTICKS__, &nbTicks, jni_int, 1);

        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_tics");
            return SET_PROPERTY_ERROR;
        }
    }
    else if (type == __GO_AXES__)
    {
        int autoSubticks;
        int i;
        double* values = (double*)_pvData;
        int nbTicks = (int) values[0];

        result = SET_PROPERTY_SUCCEED;

        if ((nbCol != 3) && (nbCol != 2))
        {
            Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "sub_tics", 2, 3);
            return SET_PROPERTY_ERROR;
        }

        /*
         * The AUTO_SUBTICKS property is shared by the 3 axes for now.
         * To be modified.
         */
        autoSubticks = nbTicks < 0 ? 1 : 0;
        status = setGraphicObjectProperty(iObjUID, __GO_AUTO_SUBTICKS__, &autoSubticks, jni_bool, 1);

        if (status == FALSE)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_ticks");
            return SET_PROPERTY_ERROR;
        }

        for (i = 0; i < nbCol ; i++)
        {
            nbTicks = (int) values[i];

            if (nbTicks < 0)
            {
                nbTicks = 0;
            }

            status = setGraphicObjectProperty(iObjUID, axisSubticksPropertiesNames[i], &nbTicks, jni_int, 1);

            if (status == FALSE)
            {
                result = SET_PROPERTY_ERROR;
            }
        }

        if (result == SET_PROPERTY_ERROR)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_ticks");
        }

        return result;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_ticks");
        return SET_PROPERTY_ERROR;
    }
    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
