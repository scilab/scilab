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
/* file: get_z_ticks_property.c                                           */
/* desc : function to retrieve in Scilab the z_ticks field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "get_ticks_utils.h"
#include "sci_malloc.h"
#include "BasicAlgos.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_z_ticks_property(void* _pvCtx, int iObjUID)
{
    int iNbTicks = 0;
    int *piNbTicks = &iNbTicks;
    int iView = 0;
    int* piView = &iView;
    void* tList = NULL;

    /* retrieve number of ticks */
    getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_NUMBER_TICKS__, jni_int, (void**)&piNbTicks);
    if (piNbTicks == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "z_ticks");
        return NULL;
    }

    /* retrieve view: 0 -> 2d // 1 -> 3d */
    getGraphicObjectProperty(iObjUID, __GO_VIEW__, jni_int, (void**)&piView);
    if (piView == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "view");
        return NULL;
    }

    if (iNbTicks == 0 || iView == 0)
    {
        /* return empty matrices */
        tList = buildTListForTicks(NULL, NULL, 0);
    }
    else
    {
        char ** labels = NULL;
        double* positions = NULL;

        getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_TICKS_LOCATIONS__, jni_double_vector, (void **) &positions);

        getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_TICKS_LABELS__, jni_string_vector, (void **) &labels);

        if (positions == NULL || labels == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "z_ticks");
            return NULL;
        }

        tList = buildTListForTicks(positions, labels, iNbTicks);

        /* free arrays */
#if 0
        destroyStringArray(labels, iNbTicks);
        FREE(positions);
#endif
    }

    return tList;

}
/*------------------------------------------------------------------------*/
