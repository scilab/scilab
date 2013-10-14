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
/* file: get_y_ticks_property.c                                           */
/* desc : function to retrieve in Scilab the y_ticks field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "get_ticks_utils.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_y_ticks_property(void* _pvCtx, int iObjUID)
{
    int iNbTicks = 0;
    int *piNbTicks = &iNbTicks;

    /* retrieve number of ticks */
    getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_NUMBER_TICKS__, jni_int, (void **) &piNbTicks);

    if (piNbTicks == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "y_ticks");
        return -1;
    }

    if (iNbTicks == 0)
    {
        /* return empty matrices */
        buildTListForTicks(NULL, NULL, 0);
    }
    else
    {
        char ** labels = NULL;
        double* positions = NULL;

        getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_TICKS_LOCATIONS__, jni_double_vector, (void **) &positions);

        getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_TICKS_LABELS__, jni_string_vector, (void **) &labels);

        if (positions == NULL || labels == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "y_ticks");
            return -1;
        }

        buildTListForTicks(positions, labels, iNbTicks);

        /* free arrays */
#if 0
        destroyStringArray(labels, iNbTicks);
        FREE(positions);
#endif
    }

    return 0;

}
/*------------------------------------------------------------------------*/
