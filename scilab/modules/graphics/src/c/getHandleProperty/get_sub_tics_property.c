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
/* file: get_sub_tics_property.c                                          */
/* desc : function to retrieve in Scilab the sub_tics or sub_ticks field  */
/*        of a handle                                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_sub_tics_property(void* _pvCtx, int iObjUID)
{
    int iType = -1;
    int *piType = &iType;
    int iSubTicks = 0;
    int* piSubTicks = &iSubTicks;

    /*Dj.A 17/12/2003*/
    /* modified jb Silvy 01/2006 */

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);
    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "type");
        return NULL;
    }

    /*
     * Type test required as the Axis object stores subticks as a single int
     * whereas Axes maintain a 3-element int vector.
     */
    if (iType == __GO_AXIS__)
    {
        getGraphicObjectProperty(iObjUID, __GO_SUBTICKS__, jni_int, (void**)&piSubTicks);

        if (piSubTicks == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_ticks");
            return NULL;
        }

        return sciReturnDouble(iSubTicks);
    }
    else if (iType == __GO_AXES__)
    {
        double sub_ticks[3];
        int iView = 0;
        int* piView = &iView;

        getGraphicObjectProperty(iObjUID, __GO_X_AXIS_SUBTICKS__, jni_int, (void**)&piSubTicks);
        if (piSubTicks == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_ticks");
            return NULL;
        }

        sub_ticks[0] = iSubTicks;

        getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_SUBTICKS__, jni_int, (void**)&piSubTicks);
        if (piSubTicks == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_ticks");
            return NULL;
        }

        sub_ticks[1] = iSubTicks;

        getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_SUBTICKS__, jni_int, (void**)&piSubTicks);
        if (piSubTicks == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_ticks");
            return NULL;
        }

        sub_ticks[2] = iSubTicks;

        getGraphicObjectProperty(iObjUID, __GO_VIEW__, jni_int, (void**)&piView);
        if (piView == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "view");
            return NULL;
        }

        if (iView == 1)
        {
            return sciReturnRowVector(sub_ticks, 3);
        }
        else
        {
            return sciReturnRowVector(sub_ticks, 2);
        }
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "sub_ticks");
        return NULL;
    }
}
/*------------------------------------------------------------------------*/
