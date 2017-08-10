/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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
/* file: get_zoom_state_property.c                                        */
/* desc : function to retrieve in Scilab the zoom_state field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*------------------------------------------------------------------------*/
void* get_zoom_state_property(void* _pvCtx, int iObjUID)
{
    int iZoomState = 0;
    int *piZoomState = &iZoomState;

    getGraphicObjectProperty(iObjUID, __GO_ZOOM_ENABLED__, jni_bool, (void **)&piZoomState);

    if (piZoomState == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "zoom_state");
        return NULL;
    }

    if (iZoomState)
    {
        return sciReturnString("on");
    }
    return sciReturnString("off");
}
/*------------------------------------------------------------------------*/
