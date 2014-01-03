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
/* file: get_axes_visible_property.c                                      */
/* desc : function to retrieve in Scilab the axes_visible field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "os_strdup.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_axes_visible_property(void* _pvCtx, int iObjUID)
{
    char * axes_visible[3]  = { NULL, NULL, NULL };
    int  const axesVisiblePropertiesNames[3] = {__GO_X_AXIS_VISIBLE__, __GO_Y_AXIS_VISIBLE__, __GO_Z_AXIS_VISIBLE__};
    int iAxesVisible = 0;
    int* piAxesVisible = &iAxesVisible;

    int i = 0;
    int j = 0;
    void* status = NULL;

    for (i = 0 ; i < 3 ; i++)
    {
        getGraphicObjectProperty(iObjUID, axesVisiblePropertiesNames[i], jni_bool, (void **)&piAxesVisible);

        if (piAxesVisible == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "axes_visible");
            return NULL;
        }

        if (iAxesVisible)
        {
            axes_visible[i] = os_strdup("on");
        }
        else
        {
            axes_visible[i] = os_strdup("off");
        }

        if (axes_visible[i] == NULL)
        {
            for (j = 0 ; j < i ; j++)
            {
                FREE(axes_visible[j]);
            }

            Scierror(999, _("%s: No more memory.\n"), "get_axes_visible_property");
            return NULL;
        }

    }

    status = sciReturnRowStringVector(axes_visible, 3);

    for (i = 0 ; i < 3 ; i++)
    {
        FREE(axes_visible[i]);
    }

    return status;
}
/*------------------------------------------------------------------------*/
