/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2014 - Scilab Enterprises - Vincent COUVERT
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
/* file: set_tight_limits_property.c                                      */
/* desc : function to modify in Scilab the tight_limits field of          */
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
int set_tight_limits_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status[3];
    char ** values = NULL;
    int mSize = nbRow * nbCol;

    int const axesTightLimitsPropertiesNames[3] = {__GO_X_TIGHT_LIMITS__, __GO_Y_TIGHT_LIMITS__, __GO_Z_TIGHT_LIMITS__};

    if (mSize == 1)
    {
        int tightLimits = (int) FALSE;

        if (valueType == sci_strings)
        {
            tightLimits = tryGetBooleanValueFromStack(((char **)_pvData)[0], valueType, nbRow, nbCol, "tight_limits");
        }
        else
        {
            tightLimits = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "tight_limits");
        }

        if (tightLimits == NOT_A_BOOLEAN_VALUE)
        {
            return SET_PROPERTY_ERROR;
        }

        status[0] = setGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[0], &tightLimits, jni_bool, 1);
        status[1] = setGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[1], &tightLimits, jni_bool, 1);
        status[2] = setGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[2], &tightLimits, jni_bool, 1);

        if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "tight_limits");
            return SET_PROPERTY_ERROR;
        }
    }
    else if (mSize == 2 || mSize == 3)
    {
        int i;
        BOOL tightLimits[3];
        int iTightLimits = 0;
        int *piTightLimits = &iTightLimits;

        if (valueType != sci_strings)
        {
            Scierror(999, _("Wrong type for '%s' property: string expected.\n"), "tight_limits");
            return SET_PROPERTY_ERROR;
        }

        getGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[0], jni_bool, (void **)&piTightLimits);

        if (piTightLimits == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "tight_limits");
            return SET_PROPERTY_ERROR;
        }

        tightLimits[0] = iTightLimits;

        getGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[1], jni_bool, (void **)&piTightLimits);
        tightLimits[1] = iTightLimits;

        getGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[2], jni_bool, (void **)&piTightLimits);
        tightLimits[2] = iTightLimits;

        values = (char**)_pvData;

        for (i = 0; i < mSize; i++)
        {
            if (strcmp(values[i], "off") == 0)
            {
                tightLimits[i] = FALSE;
            }
            else if (strcmp(values[i], "on") == 0)
            {
                tightLimits[i] = TRUE;
            }
            else
            {
                Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "tight_limits", "on", "off");
                return SET_PROPERTY_ERROR;
            }
        }

        status[0] = setGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[0], &tightLimits[0], jni_bool, 1);
        status[1] = setGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[1], &tightLimits[1], jni_bool, 1);
        status[2] = setGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[2], &tightLimits[2], jni_bool, 1);

        if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "tight_limits");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        Scierror(999, _("Wrong size for '%s' property: At most %d elements expected.\n"), "tight_limits", 3);
        return SET_PROPERTY_ERROR;
    }
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/

