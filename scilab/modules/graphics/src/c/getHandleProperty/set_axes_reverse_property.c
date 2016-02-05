/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: set_axes_reverse_property.c                                      */
/* desc : function to modify in Scilab the axes_reverse field of          */
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
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_axes_reverse_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status[3];
    int axesReversePropertiesNames[3] = {__GO_X_AXIS_REVERSE__, __GO_Y_AXIS_REVERSE__, __GO_Z_AXIS_REVERSE__};
    BOOL reverse = FALSE;

    char ** values = (char**)_pvData;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: string expected.\n"), "axes_reverse");
        return SET_PROPERTY_ERROR;
    }

    if (nbCol == 1)
    {
        /* only one parameter to set the value for every axes.*/
        if (strcmp(values[0], "off") == 0)
        {
            reverse = FALSE;
        }
        else if (strcmp(values[0], "on") == 0)
        {
            reverse = TRUE;
        }
        else
        {
            Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "axes_reverse", "on", "off");
            return SET_PROPERTY_ERROR ;;
        }

        status[0] = setGraphicObjectProperty(iObjUID, axesReversePropertiesNames[0], &reverse, jni_bool, 1);
        status[1] = setGraphicObjectProperty(iObjUID, axesReversePropertiesNames[1], &reverse, jni_bool, 1);
        status[2] = setGraphicObjectProperty(iObjUID, axesReversePropertiesNames[2], &reverse, jni_bool, 1);

        if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "axes_reverse");
            return SET_PROPERTY_ERROR;
        }
    }
    else if (nbCol == 2 || nbCol == 3)
    {
        int i;
        int result = SET_PROPERTY_SUCCEED;

        for (i = 0; i < nbCol; i++)
        {
            if (strcmp(values[i], "off") == 0)
            {
                reverse = FALSE;
            }
            else if (strcmp(values[i], "on") == 0)
            {
                reverse = TRUE;
            }
            else
            {
                Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "axes_reverse", "on", "off");
                return SET_PROPERTY_ERROR;
            }

            status[i] = setGraphicObjectProperty(iObjUID, axesReversePropertiesNames[i], &reverse, jni_bool, 1);

            if (status[i] != TRUE)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "axes_reverse");
                result = SET_PROPERTY_ERROR;
            }

        }

        return result;
    }
    else
    {
        Scierror(999, _("Wrong size for '%s' property: At most %d elements expected.\n"), "axes_reverse", 3);
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
