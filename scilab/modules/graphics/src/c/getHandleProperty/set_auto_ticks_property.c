/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: set_auto_ticks_property.c                                        */
/* desc : function to modify in Scilab the auto_ticks field of            */
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
int set_auto_ticks_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status[3];
    BOOL autoTicks = FALSE;
    char ** values = NULL;
    int mSize = nbRow * nbCol;

    int const axesAutoTicksPropertiesNames[3] = {__GO_X_AXIS_AUTO_TICKS__, __GO_Y_AXIS_AUTO_TICKS__, __GO_Z_AXIS_AUTO_TICKS__};

    if ( !( valueType == sci_strings ) )
    {
        Scierror(999, _("Wrong type for '%s' property: String matrix expected.\n"), "auto_ticks");
        return SET_PROPERTY_ERROR ;
    }

    values = getStringMatrixFromStack( stackPointer );

    if ( mSize == 1 )
    {
        /* only one parameter to set the value for every axes.*/
        if ( strcmp( values[0], "off" ) == 0 )
        {
            autoTicks = FALSE;
        }
        else if ( strcmp( values[0], "on" ) == 0 )
        {
            autoTicks = TRUE;
        }
        else
        {
            Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "auto_ticks", "on", "off");
            return SET_PROPERTY_ERROR ; ;
        }

        status[0] = setGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[0], &autoTicks, jni_bool, 1);
        status[1] = setGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[1], &autoTicks, jni_bool, 1);
        status[2] = setGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[2], &autoTicks, jni_bool, 1);

        if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_ticks");
            return SET_PROPERTY_ERROR;
        }
    }
    else if ( mSize == 2 || mSize == 3)
    {
        int i ;
        BOOL autoTicks[3];
        int iAutoTicks = 0;
        int *piAutoTicks = &iAutoTicks;

        getGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[0], jni_bool, (void **)&piAutoTicks);

        if (piAutoTicks == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_ticks");
            return SET_PROPERTY_ERROR;
        }

        autoTicks[0] = iAutoTicks;

        getGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[1], jni_bool, (void **)&piAutoTicks);
        autoTicks[1] = iAutoTicks;

        getGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[2], jni_bool, (void **)&piAutoTicks);
        autoTicks[2] = iAutoTicks;

        for ( i = 0; i < mSize; i++ )
        {
            if ( strcmp(values[i], "off") == 0 )
            {
                autoTicks[i] = FALSE;
            }
            else if ( strcmp(values[i], "on") == 0 )
            {
                autoTicks[i] = TRUE;
            }
            else
            {
                Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), "auto_ticks", "on", "off");
                return SET_PROPERTY_ERROR ;
            }
        }

        status[0] = setGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[0], &autoTicks[0], jni_bool, 1);
        status[1] = setGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[1], &autoTicks[1], jni_bool, 1);
        status[2] = setGraphicObjectProperty(pobjUID, axesAutoTicksPropertiesNames[2], &autoTicks[2], jni_bool, 1);

        if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_ticks");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        Scierror(999, _("Wrong size for '%s' property: At most %d elements expected.\n"), "auto_ticks", 3);
        return SET_PROPERTY_ERROR ;
    }
    return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
