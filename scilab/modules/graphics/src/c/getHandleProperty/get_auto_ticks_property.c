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
/* file: get_auto_ticks_property.c                                        */
/* desc : function to retrieve in Scilab the auto_ticks field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_auto_ticks_property(void* _pvCtx, int iObjUID)
{
    char * auto_ticks[3]  = { NULL, NULL, NULL };
    int const axesAutoTicksPropertiesNames[3] = {__GO_X_AXIS_AUTO_TICKS__, __GO_Y_AXIS_AUTO_TICKS__, __GO_Z_AXIS_AUTO_TICKS__};
    int iAutoTicks = 0;
    int* piAutoTicks = &iAutoTicks;

    int i = 0;
    int j = 0;
    int status = -1;

    for (i = 0 ; i < 3 ; i++)
    {
        getGraphicObjectProperty(iObjUID, axesAutoTicksPropertiesNames[i], jni_bool, (void **)&piAutoTicks);

        if (piAutoTicks == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_ticks");
            return -1;
        }

        if (iAutoTicks)
        {
            auto_ticks[i] = strdup("on");
        }
        else
        {
            auto_ticks[i] = strdup("off");
        }

        if (auto_ticks[i] == NULL)
        {
            for (j = 0 ; j < i ; j++)
            {
                FREE(auto_ticks[j]);
            }

            Scierror(999, _("%s: No more memory.\n"), "get_auto_ticks_property");
            return -1;
        }

    }

    status = sciReturnRowStringVector(_pvCtx, auto_ticks, 3);

    for (i = 0 ; i < 3 ; i++)
    {
        FREE(auto_ticks[i]);
    }

    return status;
}
/*------------------------------------------------------------------------*/
