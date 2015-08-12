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
/* file: get_axes_reverse_property.c                                      */
/* desc : function to retrieve in Scilab the axes_reverse field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "getHandleProperty.h"
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
int get_axes_reverse_property(void* _pvCtx, int iObjUID)
{
    int const axesReversePropertiesNames[3] = {__GO_X_AXIS_REVERSE__, __GO_Y_AXIS_REVERSE__, __GO_Z_AXIS_REVERSE__};
    char * axes_reverse[3]  = { NULL, NULL, NULL };
    int iAxesReverse = 0;
    int* piAxesReverse = &iAxesReverse;

    int i = 0;
    int j = 0;
    int status = -1;

    for (i = 0 ; i < 3 ; i++)
    {
        getGraphicObjectProperty(iObjUID, axesReversePropertiesNames[i], jni_bool, (void **)&piAxesReverse);

        if (piAxesReverse == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "axes_reverse");
            return -1;
        }

        if (iAxesReverse)
        {
            axes_reverse[i] = strdup("on");
        }
        else
        {
            axes_reverse[i] = strdup("off");
        }

        if (axes_reverse[i] == NULL)
        {
            for (j = 0 ; j < i ; j++)
            {
                FREE(axes_reverse[j]);
            }

            Scierror(999, _("%s: No more memory.\n"), "get_axes_reverse_property");
            return -1;
        }

    }

    status = sciReturnRowStringVector(_pvCtx, axes_reverse, 3);

    for (i = 0 ; i < 3 ; i++)
    {
        FREE(axes_reverse[i]);
    }

    return status;
}
/*------------------------------------------------------------------------*/
