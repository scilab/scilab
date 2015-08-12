/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int get_log_flags_property(void* _pvCtx, int iObjUID)
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
        return -1;
    }

    logFlags[0] = iLogFlag;

    getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    if (piLogFlag == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "log_flag");
        return -1;
    }

    logFlags[1] = iLogFlag;

    getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    if (piLogFlag == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "log_flag");
        return -1;
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
    return sciReturnString(_pvCtx, logFlagsString);
}
/*------------------------------------------------------------------------*/
