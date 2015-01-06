/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 * Copyright (C) 2014 - Scilab Enterprises - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_tight_limits_property.c                                      */
/* desc : function to retrieve in Scilab the tight_limits field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_tight_limits_property(void* _pvCtx, int iObjUID)
{
    char * tightLimits[3]  = { NULL, NULL, NULL };
    int const axesTightLimitsPropertiesNames[3] = {__GO_X_TIGHT_LIMITS__, __GO_Y_TIGHT_LIMITS__, __GO_Z_TIGHT_LIMITS__};
    int iTightLimits = 0;
    int* piTightLimits = &iTightLimits;

    int i = 0;
    int j = 0;
    int status = -1;

    for (i = 0 ; i < 3 ; i++)
    {
        getGraphicObjectProperty(iObjUID, axesTightLimitsPropertiesNames[i], jni_bool, (void **)&piTightLimits);

        if (piTightLimits == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "tight_limits");
            return -1;
        }

        if (iTightLimits)
        {
            tightLimits[i] = strdup("on");
        }
        else
        {
            tightLimits[i] = strdup("off");
        }

        if (tightLimits[i] == NULL)
        {
            for (j = 0 ; j < i ; j++)
            {
                free(tightLimits[j]);
            }

            Scierror(999, _("%s: No more memory.\n"), "get_tight_limits_property");
            return -1;
        }

    }

    status = sciReturnRowStringVector(_pvCtx, tightLimits, 3);

    for (i = 0 ; i < 3 ; i++)
    {
        free(tightLimits[i]);
    }

    return status;
}
/*------------------------------------------------------------------------*/
