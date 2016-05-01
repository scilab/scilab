/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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
/* file: get_log_flags_property.c                                         */
/* desc : function to retrieve in Scilab the log_flags field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_log_flags_property(void* _pvCtx, int iObjUID)
{
    int i = 0;
    int iLogFlag = 0;
    int* piLogFlag = &iLogFlag;
    int logFlags[3];
    char logFlagsString[4];

    getGraphicObjectProperty(iObjUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);

    if (piLogFlag == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "log_flag");
        return NULL;
    }

    logFlags[0] = iLogFlag;

    getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    if (piLogFlag == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "log_flag");
        return NULL;
    }

    logFlags[1] = iLogFlag;

    getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    if (piLogFlag == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "log_flag");
        return NULL;
    }

    logFlags[2] = iLogFlag;

    for (i = 0; i < 3; i++)
    {
        if (logFlags[i] == 0)
        {
            logFlagsString[i] = 'n';
        }
        else
        {
            logFlagsString[i] = 'l';
        }
    }

    /* 0 terminating character */
    logFlagsString[3] = 0;
    return sciReturnString(logFlagsString);
}
/*------------------------------------------------------------------------*/
