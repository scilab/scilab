/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int get_zoom_state_property(void* _pvCtx, int iObjUID)
{
    int iZoomState = 0;
    int *piZoomState = &iZoomState;

    getGraphicObjectProperty(iObjUID, __GO_ZOOM_ENABLED__, jni_bool, (void **)&piZoomState);

    if (piZoomState == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "zoom_state");
        return -1;
    }

    if (iZoomState)
    {
        return sciReturnString(_pvCtx, "on");
    }
    return sciReturnString(_pvCtx, "off");
}
/*------------------------------------------------------------------------*/
